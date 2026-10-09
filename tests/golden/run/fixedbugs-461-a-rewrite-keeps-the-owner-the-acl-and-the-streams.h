/* Defect 461's fixture: what a file is besides its bytes, put on it and
 * asked again, by each platform's own means. On Windows an explicit ACE
 * granting Everyone read, a stream of the mark of the web
 * (`Zone.Identifier`) and the hidden attribute, the three a rename lost
 * there, and the owner, which `keeps` holds to what it was; elsewhere an
 * extended attribute, where the filesystem keeps one, and the mode 0640,
 * which `parts/replace.c` carries on POSIX. `mark` answers 1 when the file
 * was marked, `keeps` 1 when every mark is still there. */
#include <stdint.h>
#include <string.h>
#if defined(_WIN32)
#include <windows.h>
#include <aclapi.h>
#include <sddl.h>
/* The security calls are advapi32's, which a link takes only when a source
 * asks for it; asked here, as the runtime's `parts/replace.c` asks. */
#pragma comment(lib, "advapi32.lib")

static char fixture_owner[256];

static int64_t fixture_owner_of(const char *path, char *out, size_t room) {
    PSID owner = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;
    if (GetNamedSecurityInfoA(path, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &owner, NULL, NULL, NULL, &sd) !=
        ERROR_SUCCESS) return 0;
    LPSTR s = NULL;
    int made = ConvertSidToStringSidA(owner, &s);
    if (made) {
        strncpy(out, s, room - 1);
        out[room - 1] = '\0';
        LocalFree(s);
    }
    LocalFree(sd);
    return made ? 1 : 0;
}

static inline int64_t mark(const char *path) {
    char zone[1024];
    if (strlen(path) + 32 > sizeof zone) return 0;
    strcpy(zone, path);
    strcat(zone, ":Zone.Identifier");
    HANDLE h = CreateFileA(zone, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return 0;
    DWORD w = 0;
    BOOL put = WriteFile(h, "[ZoneTransfer]\r\nZoneId=3\r\n", 26, &w, NULL);
    CloseHandle(h);
    if (!put) return 0;
    PACL old = NULL, fresh = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;
    if (GetNamedSecurityInfoA(path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &old, NULL, &sd) !=
        ERROR_SUCCESS) return 0;
    /* Everyone by its SID, which no locale renames. */
    PSID everyone = NULL;
    SID_IDENTIFIER_AUTHORITY world = SECURITY_WORLD_SID_AUTHORITY;
    if (!AllocateAndInitializeSid(&world, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyone)) {
        LocalFree(sd);
        return 0;
    }
    EXPLICIT_ACCESSA ea;
    memset(&ea, 0, sizeof ea);
    ea.grfAccessPermissions = GENERIC_READ;
    ea.grfAccessMode = GRANT_ACCESS;
    ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
    ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
    ea.Trustee.ptstrName = (LPSTR)everyone;
    DWORD set = SetEntriesInAclA(1, &ea, old, &fresh);
    FreeSid(everyone);
    LocalFree(sd);
    if (set != ERROR_SUCCESS) return 0;
    set = SetNamedSecurityInfoA((LPSTR)path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, fresh, NULL);
    LocalFree(fresh);
    if (set != ERROR_SUCCESS) return 0;
    if (!SetFileAttributesA(path, FILE_ATTRIBUTE_HIDDEN)) return 0;
    return fixture_owner_of(path, fixture_owner, sizeof fixture_owner);
}

static inline int64_t keeps(const char *path) {
    char zone[1024];
    strcpy(zone, path);
    strcat(zone, ":Zone.Identifier");
    if (GetFileAttributesA(zone) == INVALID_FILE_ATTRIBUTES) return 0;
    if ((GetFileAttributesA(path) & FILE_ATTRIBUTE_HIDDEN) == 0) return 0;
    char now[256];
    if (!fixture_owner_of(path, now, sizeof now) || strcmp(now, fixture_owner) != 0) return 0;
    PACL dacl = NULL;
    PSECURITY_DESCRIPTOR sd = NULL;
    if (GetNamedSecurityInfoA(path, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &dacl, NULL, &sd) !=
        ERROR_SUCCESS) return 0;
    int64_t explicit_aces = 0;
    for (DWORD i = 0; dacl != NULL && i < dacl->AceCount; i++) {
        ACE_HEADER *ace = NULL;
        if (GetAce(dacl, i, (void **)&ace) && (ace->AceFlags & INHERITED_ACE) == 0) explicit_aces++;
    }
    LocalFree(sd);
    return explicit_aces == 1 ? 1 : 0;
}

/* The attribute off again, so the case's own clean-up can remove it. */
static inline void unmark(const char *path) { SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL); }
#else
#include <sys/stat.h>
#include <sys/xattr.h>
#include <errno.h>

#if defined(__APPLE__)
#define FIXTURE_XATTR "com.heroes.fixture461"
static int fixture_set(const char *p) { return setxattr(p, FIXTURE_XATTR, "3", 1, 0, 0); }
static ssize_t fixture_get(const char *p, char *v, size_t n) { return getxattr(p, FIXTURE_XATTR, v, n, 0, 0); }
#else
#define FIXTURE_XATTR "user.heroes.fixture461"
static int fixture_set(const char *p) { return setxattr(p, FIXTURE_XATTR, "3", 1, 0); }
static ssize_t fixture_get(const char *p, char *v, size_t n) { return getxattr(p, FIXTURE_XATTR, v, n); }
#endif

static int fixture_has_xattr;

static inline int64_t mark(const char *path) {
    if (chmod(path, 0640) != 0) return 0;
    fixture_has_xattr = fixture_set(path) == 0;
    return fixture_has_xattr || errno == ENOTSUP || errno == EOPNOTSUPP ? 1 : 0;
}

static inline int64_t keeps(const char *path) {
    struct stat info;
    if (stat(path, &info) != 0 || (info.st_mode & 07777) != 0640) return 0;
    if (!fixture_has_xattr) return 1;
    char v[8];
    return fixture_get(path, v, sizeof v) == 1 && v[0] == '3' ? 1 : 0;
}

static inline void unmark(const char *path) { (void)path; }
#endif
