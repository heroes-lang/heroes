/* The hand-written case of defect 463, run on a Linux with SELinux enforcing
 * (the defect exists only there, so no golden of the net can hold it; the
 * net's legs, this Mac, Linux arm64 in Docker whose kernel has no SELinux, the
 * CI's Linux and the Windows box, are not where it can be asked).
 *
 * WHAT IT ASKS. What the runtime's two doors keep of a file's `security.*`
 * attributes when they rewrite it: `write NAME` is the program's `write_file`
 * (`hero_file_write`), `publish NAME` the publish route (`hero_file_stage`,
 * then `hero_fs_replace`), `inplace NAME` the `fopen("wb")` write both
 * replaced, for comparison. It prints the attributes before and after, each
 * door's answer and why, and whether the inode changed.
 *
 * THE MACHINE IT WAS RUN ON, 2026-10-08, from the Mac: Fedora Cloud 44
 * aarch64 (`Fedora-Cloud-Base-Generic-44-1.7.aarch64.qcow2`, sha256
 * 55c60a3b80d3616a08705afd0459e75fe9f03c54aba7a46e4002a41a72fa0d5b), booted
 * by `qemu-system-aarch64 -machine virt -accel hvf` with a cloud-init seed
 * served over `-smbios type=1,serial=ds=nocloud;s=http://10.0.2.2:8000/`;
 * SELinux enforcing, policy targeted; `dnf install clang attr
 * policycoreutils-python-utils libcap`. A second account confined by
 * `semanage login -a -s user_u conf`, whose processes run as
 * `user_u:user_r:user_t:s0` and may not set a label of another SELinux user.
 *
 * THE SHAPES AND WHAT THEY MEASURED (rows are the file's label before, the
 * label the new file is born with, the process):
 *   n1  unconfined_u:object_r:user_tmp_t  ->  user_u:object_r:user_home_t,
 *       confined: before the repair BOTH doors left `user_u:object_r:
 *       user_home_t:s0`, in silence, where `inplace` kept the label; after,
 *       `write` goes in place (same inode, label kept) and `publish`'s stage
 *       answers 5 (attributes), why 13 (EACCES), the file untouched.
 *   n2  user_u:object_r:user_tmp_t, confined: carried before and after.
 *   n3  the label the new file is born with: unchanged before and after.
 *   n4  unconfined, a label the process may set: carried before and after.
 *   n5  `security.capability` on a file, as root: before the repair both doors
 *       gave it to the new bytes, where `inplace` drops it (the kernel does);
 *       after, neither carries it.
 *
 * clang -std=gnu11 -I runtime docs/platforms/linux/selinux-labels.c
 *       runtime/runtime.c -o selinux-labels -lpthread -lm */
#include "heroes_runtime.h"
#include "hero_os.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <unistd.h>

static void attrs(const char *path, const char *when) {
    char names[4096];
    ssize_t n = listxattr(path, names, sizeof names);
    printf("  %s:", when);
    if (n <= 0) {
        printf(" (none)\n");
        return;
    }
    for (ssize_t at = 0; at < n; at += (ssize_t)strlen(names + at) + 1) {
        char value[512];
        ssize_t got = getxattr(path, names + at, value, sizeof value - 1);
        if (got < 0) {
            printf(" %s=<unreadable>", names + at);
            continue;
        }
        value[got] = 0;
        int printable = 1;
        for (ssize_t k = 0; k < got; k++) {
            if (value[k] != 0 && (value[k] < 32 || value[k] > 126)) printable = 0;
        }
        if (printable) {
            printf(" %s=%s", names + at, value);
        } else {
            printf(" %s=<%zd bytes>", names + at, got);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc < 3) return 2;
    hero_args_set(argc, argv);
    const char *how = argv[1];
    const char *path = argv[2];
    HeroStr text = hero_str_from_bytes("NEW TEXT\n", 9);
    struct stat before, after;
    if (stat(path, &before) != 0) {
        printf("  cannot stat %s\n", path);
        return 2;
    }
    attrs(path, "before");
    if (strcmp(how, "write") == 0) {
        int64_t r = hero_file_write(path, text);
        printf("  hero_file_write: %lld, why %lld\n", (long long)r, (long long)hero_fs_why());
    } else if (strcmp(how, "publish") == 0) {
        char staged[1024];
        snprintf(staged, sizeof staged, "%s.staged", path);
        int64_t s = hero_file_stage(staged, text, path);
        printf("  hero_file_stage: %lld, why %lld\n", (long long)s, (long long)hero_fs_why());
        if (s == HERO_STAGE_DONE) {
            int64_t m = hero_fs_replace(staged, path);
            printf("  hero_fs_replace: %lld, why %lld\n", (long long)m, (long long)hero_fs_why());
        }
    } else {
        FILE *f = fopen(path, "wb");
        if (f == NULL) {
            printf("  fopen: %d\n", errno);
        } else {
            fputs("NEW TEXT\n", f);
            fclose(f);
        }
    }
    if (stat(path, &after) != 0) {
        printf("  gone\n");
        return 1;
    }
    attrs(path, "after");
    printf("  same inode: %s\n", before.st_ino == after.st_ino ? "yes" : "no");
    hero_str_decref(text);
    return 0;
}
