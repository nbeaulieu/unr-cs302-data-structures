# CS 302 Data Structures — Interactive Visualizations

Academic website and interactive sorting algorithm visualizations for CS 302 Data Structures course at University of Nevada, Reno.

## 🎯 Features

- **Professional Academic Website**: Clean, modern homepage showcasing courses, research, and teaching materials
- **Interactive Algorithm Visualizations**: Step-by-step animations for algorithms and data structures
  - **Sorting Algorithms**: Selection Sort, Bubble Sort (with early termination), Insertion Sort
  - **Data Structures**: Binary Search Trees (insert, search, balanced vs degenerate)
- **Accessibility**: Full keyboard navigation, ARIA labels, and screen reader support
- **Responsive Design**: Works beautifully on desktop, tablet, and mobile devices
- **GitHub Pages Ready**: Configured for easy deployment

## 🚀 Getting Started

### Local Development

Simply open `index.html` in your web browser. All files are self-contained with inline CSS and JavaScript.

### Deploy to GitHub Pages

1. Push this repository to GitHub
2. Go to Settings → Pages
3. Select your branch (usually `main` or `master`)
4. Your site will be live at `https://nbeaulieu.github.io/unr-cs302-data-structures/`

## 📁 Project Structure

```
.
├── index.html                      # Main homepage
├── sorting/
│   ├── selection_sort.html         # Selection sort visualization
│   ├── bubble_sort.html            # Bubble sort visualization
│   └── insertion_sort.html         # Insertion sort visualization
├── data-structures/
│   └── binary_search_tree.html     # Binary search tree visualization
├── .nojekyll                       # GitHub Pages config (no Jekyll)
├── .gitignore                      # Git ignore rules
└── README.md                       # This file
```

## 🎨 Customization

### Update Personal Information

All personal information has been configured:
- ✅ LinkedIn: https://www.linkedin.com/in/beaulieunicole
- ✅ GitHub: https://github.com/nbeaulieu
- ✅ Open Graph URL configured

### Add More Visualizations

1. Create a new HTML file in the appropriate directory (`sorting/` for algorithms, `data-structures/` for data structures)
2. Add a link in `index.html` under the "Interactive Visualizations" section
3. Follow the same structure and styling as existing visualizations
4. Ensure the back link points to `../index.html`

## ⌨️ Keyboard Shortcuts

In the sorting visualizations:
- **Space** or **→** : Step forward / Play/Pause
- **←** : Reset
- **R** : Reset

## 🛠️ Technologies

- Pure HTML, CSS, and JavaScript
- No dependencies or build process required
- Google Fonts (DM Sans, Source Serif 4, JetBrains Mono)

## 📝 License

This project is for educational use. Feel free to adapt and modify for your own courses.

## 👤 Author

**Nicole Beaulieu, Ph.D.**  
Part-Time Faculty · Department of Computer Science & Engineering  
University of Nevada, Reno

Software engineer with 33+ years in industry, passionate about sharing knowledge with the next generation of software engineers and computer scientists.

---

Built with ❤️ for CS education
