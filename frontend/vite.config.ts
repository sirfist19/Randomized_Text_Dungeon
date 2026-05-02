import { defineConfig, loadEnv } from 'vite';

// GitHub Pages project URL: https://<USERNAME>.github.io/Randomized_Text_Dungeon/
// The repo name becomes the path segment; USERNAME only affects the hostname.
const GITHUB_PAGES_BASE = '/Randomized_Text_Dungeon/';

/**
 * Local dev: keep `VITE_API_BASE_URL` in `.env` pointing at your Oracle (or local) API origin
 * (no path suffix, e.g. http://203.0.113.10:8765). The dev server proxies `/api` and `/health`
 * there so the browser stays same-origin with localhost.
 *
 * Production (`vite build`): `src/main.ts` calls `${VITE_API_BASE_URL}/api/...` directly.
 * Inject VITE_API_BASE_URL in CI (GitHub Actions env / repo secrets) so the static bundle
 * embeds your public Oracle URL. CORS must allow your github.io origin on the server.
 */
export default defineConfig(({ mode, command }) => {
	const env = loadEnv(mode, '.', '');

	return {
		// Dev: '/' so `npm run dev` stays at http://localhost:5173/
		// Build: subpath so assets resolve on GitHub Pages project sites
		base: command === 'serve' ? '/' : GITHUB_PAGES_BASE,
		server: {
			proxy: {
				'/api': {
					target: env.VITE_API_BASE_URL,
					changeOrigin: true,
				},
				'/health': {
					target: env.VITE_API_BASE_URL,
					changeOrigin: true,
				},
			},
		},
	};
});
