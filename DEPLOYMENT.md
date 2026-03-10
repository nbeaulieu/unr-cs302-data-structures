# GitHub Pages Deployment Guide

This project is ready to deploy to GitHub Pages. Follow these steps:

## 🚀 Quick Deployment

### Step 1: Push to GitHub

If you haven't already, create a repository and push your code:

```bash
# Initialize git (if not already done)
git init

# Add all files
git add .

# Commit
git commit -m "Initial commit: CS 302 Data Structures website"

# Add your GitHub repository as remote (replace with your repo URL)
git remote add origin https://github.com/nbeaulieu/unr-cs302-data-structures.git

# Push to GitHub
git branch -M main
git push -u origin main
```

### Step 2: Enable GitHub Pages

1. Go to your repository on GitHub: `https://github.com/nbeaulieu/unr-cs302-data-structures`
2. Click **Settings** (in the repository menu)
3. Scroll down to **Pages** (in the left sidebar)
4. Under **Source**, select:
   - **Branch**: `main` (or `master` if that's your default branch)
   - **Folder**: `/ (root)`
5. Click **Save**

### Step 3: Wait for Deployment

GitHub Pages will build and deploy your site. This usually takes 1-2 minutes.

### Step 4: Access Your Site

Your site will be live at:
**https://nbeaulieu.github.io/unr-cs302-data-structures/**

## ✅ Pre-Deployment Checklist

- [x] `.nojekyll` file exists (tells GitHub Pages not to use Jekyll)
- [x] All HTML files are in place
- [x] All links are correct (relative paths)
- [x] `.gitignore` configured to exclude unnecessary files
- [x] README.md updated with correct information
- [x] Personal information updated in `index.html`

## 🔧 Troubleshooting

### Site not updating?
- Wait a few minutes for GitHub Pages to rebuild
- Check the **Actions** tab for any build errors
- Clear your browser cache

### 404 errors?
- Verify all file paths are correct
- Check that `.nojekyll` file exists
- Ensure files are committed and pushed to GitHub

### Links not working?
- All links should use relative paths (e.g., `../index.html`, not absolute URLs)
- Check that all referenced files exist in the repository

## 📝 Notes

- GitHub Pages serves static files only (HTML, CSS, JavaScript)
- No server-side processing required
- All visualizations are client-side JavaScript
- Site updates automatically when you push to the main branch

## 🔗 Useful Links

- [GitHub Pages Documentation](https://docs.github.com/en/pages)
- [Your Repository](https://github.com/nbeaulieu/unr-cs302-data-structures)
- [Your Live Site](https://nbeaulieu.github.io/unr-cs302-data-structures/)
