# Recommended Directory Structure for GitHub Pages

## Proposed Structure

```
unr-cs302-data-structures/
├── index.html                      # Main website homepage
│
├── sorting/                        # Sorting algorithm visualizations
│   ├── selection_sort.html
│   ├── bubble_sort.html
│   └── insertion_sort.html
│
├── data-structures/                # Data structure visualizations
│   └── binary_search_tree.html
│
├── code/                           # C++ code examples for teaching
│   ├── selection_sort.cpp
│   ├── bubble_sort.cpp            # (future)
│   ├── insertion_sort.cpp         # (future)
│   └── README.md                  # Code examples documentation
│
├── docs/                           # Project documentation
│   ├── DEPLOYMENT.md
│   ├── GITHUB_PAGES_SETUP.md
│   └── CHECKLIST.md
│
├── .nojekyll                       # GitHub Pages config
├── .gitignore                      # Git ignore rules
└── README.md                       # Main project README
```

## Rationale

### ✅ Keep in Root:
- `index.html` - Must be in root for GitHub Pages to work as homepage
- `README.md` - GitHub displays this on the repo page
- `.nojekyll`, `.gitignore` - Configuration files belong in root

### ✅ Organize by Purpose:

1. **Visualizations** (`sorting/`, `data-structures/`)
   - HTML files that are part of the website
   - Students access these via the website
   - Already organized by topic

2. **Code Examples** (`code/`)
   - C++ source files for teaching
   - Separate from website content
   - Easy to add more examples (bubble_sort.cpp, etc.)
   - Can include a README explaining how to compile/run

3. **Documentation** (`docs/`)
   - Deployment guides, setup instructions
   - Keeps root directory clean
   - Easy to find all documentation in one place

## Benefits

- **Clear separation**: Website vs teaching materials vs docs
- **Scalable**: Easy to add more code examples or visualizations
- **Professional**: Organized structure looks clean
- **Student-friendly**: Code examples are easy to find
- **Maintainable**: Each type of content has its own place

## Alternative Structure (if you prefer)

If you want code examples closer to their visualizations:

```
├── sorting/
│   ├── selection_sort.html
│   └── code/
│       └── selection_sort.cpp
├── data-structures/
│   ├── binary_search_tree.html
│   └── code/
│       └── binary_search_tree.cpp  # (future)
```

This keeps related files together but creates more nested directories.
