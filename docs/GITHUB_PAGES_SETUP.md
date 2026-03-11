# GitHub Pages Setup Instructions

## ✅ Repository Status
- ✅ Repository exists: `nbeaulieu/unr-cs302-data-structures`
- ✅ Code pushed to `develop` branch
- ✅ Code pushed to `main` branch
- ⚠️ GitHub Pages needs to be enabled

## 🔧 Enable GitHub Pages (Step-by-Step)

### Method 1: Via GitHub Web Interface (Recommended)

1. **Go to your repository:**
   - Open: https://github.com/nbeaulieu/unr-cs302-data-structures

2. **Navigate to Settings:**
   - Click the **Settings** tab (top menu, right side)

3. **Go to Pages:**
   - In the left sidebar, scroll down and click **Pages**

4. **Configure Source:**
   - Under **Source**, select:
     - **Branch**: `main` (or `develop` if you prefer)
     - **Folder**: `/ (root)`
   - Click **Save**

5. **Wait for deployment:**
   - GitHub will show: "Your site is ready to be published at..."
   - Wait 1-2 minutes for the first deployment
   - Refresh the Pages settings page to see the URL

6. **Access your site:**
   - Your site will be live at: **https://nbeaulieu.github.io/unr-cs302-data-structures/**

### Method 2: Verify Files Are Present

Make sure these files are in your repository:
- ✅ `index.html` (in root)
- ✅ `.nojekyll` (in root)
- ✅ `sorting/` directory with HTML files
- ✅ `data-structures/` directory with HTML files

## 🔍 Troubleshooting

### If you see "There isn't a GitHub Pages site here"

1. **Check repository visibility:**
   - Repository must be **Public** OR
   - If Private, you need GitHub Pro/Team/Enterprise

2. **Verify branch exists:**
   - Go to your repo → Click "branches"
   - Make sure `main` or `develop` branch exists

3. **Check Settings → Pages:**
   - Make sure you're in the correct repository
   - Settings → Pages should show configuration options

### If Pages shows "Check again later"

- First deployment can take 5-10 minutes
- Check the **Actions** tab for deployment status
- Look for any error messages

### If site shows 404

- Wait a few more minutes
- Clear browser cache
- Verify `index.html` exists in root
- Check that `.nojekyll` file exists

## 📝 Quick Checklist

- [ ] Repository is public (or you have GitHub Pro)
- [ ] `main` branch exists and has files
- [ ] `index.html` is in the root directory
- [ ] `.nojekyll` file exists
- [ ] Settings → Pages is configured
- [ ] Selected branch: `main` (or `develop`)
- [ ] Selected folder: `/ (root)`
- [ ] Clicked "Save"

## 🎯 Expected Result

After enabling, you should see:
- A green checkmark in Settings → Pages
- A message: "Your site is live at https://nbeaulieu.github.io/unr-cs302-data-structures/"
- The site loads when you visit the URL

## 💡 Need Help?

If you're still having issues:
1. Check GitHub Status: https://www.githubstatus.com/
2. Verify repository URL: https://github.com/nbeaulieu/unr-cs302-data-structures
3. Make sure you're logged into the correct GitHub account
