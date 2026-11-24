# FoRgE Website

The FoRgE landing page is built with Vue 3, TypeScript, Vite, and Tailwind CSS v4.

## Development

```bash
cd WebSite
npm install
npm run dev
```

## Production build

```bash
npm run build
npm run preview
```

The production output is generated in `dist/`.

## Deployment

The workflow at `.github/workflows/website-pages.yml` builds the site and deploys `WebSite/dist` to GitHub Pages whenever files in `WebSite/` change on `main`.

In the repository settings, open **Pages** and set **Build and deployment → Source** to **GitHub Actions** if it is not already selected.
