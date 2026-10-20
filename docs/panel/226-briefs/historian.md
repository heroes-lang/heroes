# Panel 226, the historian's brief

Read `00-shared.md` first. You have no copy of the tree and need none; your folder is
`<scratchpad>/226-historian/` for notes. Write your report as you go to
`docs/panel/226-reports/historian.md` in the repository's lane
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/thesis/`. Every claim verified on the web
with its URL and the day read, or marked `unverified`. No paid run.

The sitting measures, for the first time, whether a language whose compiler refuses the mistakes a
model plausibly makes raises the rate at which the model's programs pass hidden tests within a few
turns of the compiler's feedback. Who has done the pieces before, and what happened to them?

1. **Code benchmarks and their estimators**: Chen et al. 2021 (HumanEval, the unbiased pass@k
   estimator; the brief names the primary `green@k` to avoid that word: is the distinction one the
   literature already draws, e.g. pass@k with feedback, *self-repair*, *turns*?); Austin et al. 2021
   (MBPP); Cassano et al., MultiPL-E (2022/2023) and **MultiPL-T** (arXiv 2308.09895, low-resource
   languages: what did they measure about languages a model has rarely seen, and how large were the
   gaps?); Jimenez et al. 2023 (SWE-bench, FAIL_TO_PASS hidden tests; SWE-bench Verified's
   adjudication); LiveCodeBench (contamination handled by problem date); Aycock et al. (arXiv
   2409.19151, already in panel 133: what it says about grammars in prompts).
2. **Held-out data in public repositories**: GPQA (Rein et al. 2023) ships its test set inside a
   password-protected archive on Hugging Face to keep it out of crawls: verify, and find the
   password's publication and any statement of why; BIG-bench's canary GUID (its purpose, how it is
   checked, whether anyone reported a hit); any benchmark that encrypted a test set in a public git
   repository and later published the key, or that was burned by a leak (e.g. what happened to
   HumanEval's problems in training sets, measured by whom). The author's question stands behind
   this: *can a sealed archive in a public repository be read as closed source, and what did others
   do about it?*
3. **The statistics**: Wilson 1927; Brown, Cai and DasGupta 2001 (why Wilson over Wald); McNemar
   1947 and the exact binomial form; Hanley and Lippman-Hand 1983 (the rule of three); the cluster
   bootstrap for correlated samples per task (a source that states it for paired designs).
4. **The terms of use the brief read from secondary sources** (00-shared, *Terms of use*): find the
   official pages and say what each states today: Anthropic's consumer privacy policy or help page on
   training from Free/Pro/Max chats (the September 2025 change) and its commercial terms on API data;
   OpenAI's *How your data is used* (help 5722486) for the API and for ChatGPT, the *Include
   environments* setting of Codex, and whether Codex CLI signed in with ChatGPT is covered by the
   consumer or the API policy; and whether either provider's consumer terms forbid automated
   benchmark use of a subscription. Mark every line verified or not.
5. **Feedback protocols**: a published precedent for giving a model the compiler's message versus a
   failing test's output and measuring repair (e.g. *Self-Debugging*, *Reflexion*, *InterCode*, Olausson
   et al. 2023 *Is Self-Repair a Silver Bullet*): what did they find about the message's value?

Output the structure your definition asks for: `precedents` with each line verified/unverified,
`argument` under 120 words, `condition`. Where the project's own book would be the route, name it as
a route and cite what you found instead.
