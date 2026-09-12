- [ ] **M-package-layout step 2** | Three vocabularies now describe one module: the PATH the machine opens (`geom/point.hero`), the MODULE that identifies it in a compilation (`geom/point`), the COMPONENT that reaches C (`geompoint`) and the BINDING a call writes (`point`). Given a file `syntax/decl.hero` used from the root, say which of the four each of these produces: `module_names.stem_of`, `module_names.component_of`, `module_names.binding_of`, and the `module` field of its `FileEntry`

    **Where to look:** selfhost/module/names.hero (the module doc names all four) · selfhost/source.hero:132
    **Why it matters:** reading a value from the wrong vocabulary is a defect class rather than a slip: panel 032 D1 and two more compiled shapes at panel 099
