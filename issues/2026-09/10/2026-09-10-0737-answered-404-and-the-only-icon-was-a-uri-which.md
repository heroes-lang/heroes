- [x] **`/favicon.ico` answered 404 and the only icon was a `data:` URI**, which
    is not an address, so the crawler that draws the icon beside a search result
    could not fetch it. Three files under `site/public/`, and the bolt is now
    drawn once in the tree instead of once inside the layout.
