- [x] **Every built page's head is checked for what a search result uses**,
    `site/src/lib/seo.ts`: title, description and canonical present; description
    at most 160 characters and title at most 60, which is where a result is cut
    rather than where a sentence is long; canonical equal to the page's own
    address; no two pages sharing a title or a description. 186 pages pass. Four
    sabotages were run one at a time and each was seen red with its own message.
