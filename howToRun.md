# How to run Randomized Text Dungeon

Game data lives in the `content/` folder at the repo root. Builds assume you run commands from `src/` so the default content path `../content` resolves correctly.

## Prerequisites

- **C++ toolchain**: `g++` and `make` (C++17).
- **Web UI (optional)**: Node.js and npm for the Vite frontend.

## Terminal (CLI) game

1. Open a terminal and go to the `src/` directory:

   ```bash
   cd src
   ```

2. Build and run:

   ```bash
   make
   ./game.exe
   ```

3. Play in the terminal. To use a different content directory, set `CONTENT_ROOT` before starting (paths are relative to how you launch the binary, or use an absolute path):

   ```bash
   CONTENT_ROOT=/absolute/path/to/content ./game.exe
   ```

## HTTP API + browser UI

The browser client talks to the C++ server over REST. Run **both** the HTTP server and the frontend dev server.

### 1. Build and start the API server

From `src/`:

```bash
cd src
make http_server
./http_server
```

By default the server listens on port **8765** (8080 is often already in use on macOS, e.g. AirPlay). Use another port:

```bash
./http_server 9000
```

If you start the server from a directory where `../content` is wrong, set the content root (also configurable per session in `POST /api/sessions`):

```bash
export CONTENT_ROOT=/absolute/path/to/Randomized_Text_Dungeon/content
./http_server
```

Endpoints:

- `GET /health` — liveness
- `POST /api/sessions` — create a session (optional JSON body, e.g. `{ "contentRoot": "../content" }`)
- `POST /api/sessions/<id>/step` — body `{ "line": "your command" }`
- `DELETE /api/sessions/<id>` — remove a session

### 2. Start the frontend (dev)

In a **second** terminal, from the repo root:

```bash
cd frontend
npm install
npm run dev
```

Open the URL Vite prints (usually `http://localhost:5173`). The dev server proxies `/api` and `/health` to `http://127.0.0.1:8765`, matching the default `http_server` port. If you change the server port, edit `frontend/vite.config.ts` to match.

### 3. Production build of the frontend (optional)

```bash
cd frontend
npm run build
```

Static files are written to `frontend/dist/`. Serve that folder with any static file server; you must still run `http_server` (or another backend) for `/api`, or configure your host to proxy API requests to the C++ process.

## Clean rebuild

From `src/`:

```bash
make clean
make
make http_server
```
