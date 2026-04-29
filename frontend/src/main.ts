import './style.css';

const API_BASE = '/api';

type StepPayload = {
  sessionId?: string;
  output: string[];
  phase: string;
  awaitingInput: boolean;
  sessionEnded: boolean;
  gameWon: boolean;
  error?: string;
  clearViewport?: boolean;
};

const logEl = document.getElementById('log')!;
const form = document.getElementById('cmd-form') as HTMLFormElement;
const input = document.getElementById('cmd-input') as HTMLInputElement;

let sessionId: string | null = null;

function appendOutput(lines: string[]) {
  for (const line of lines) {
    const div = document.createElement('div');
    div.className = 'line';
    // Blank logical lines stringify as "". Empty blocks collapse; NBSP preserves row height like a terminal newline row.
    div.textContent = line === '' ? '\u00a0' : line;
    logEl.appendChild(div);
  }
  logEl.scrollTop = logEl.scrollHeight;
}

async function postJson<T>(url: string, body: unknown): Promise<T> {
  const res = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body ?? {}),
  });
  if (!res.ok) {
    const text = await res.text();
    throw new Error(text || res.statusText);
  }
  return res.json() as Promise<T>;
}

async function bootstrap() {
  logEl.textContent = '';
  const data = await postJson<StepPayload>(`${API_BASE}/sessions`, {
    contentRoot: '../content',
  });
  sessionId = data.sessionId ?? null;
  if (data.clearViewport) {
    logEl.textContent = '';
  }
  appendOutput(data.output);
  input.disabled = data.sessionEnded || data.gameWon;
}

async function sendLine(line: string) {
  if (!sessionId) return;
  const data = await postJson<StepPayload>(`${API_BASE}/sessions/${encodeURIComponent(sessionId)}/step`, {
    line,
  });
  if (data.clearViewport) {
    logEl.textContent = '';
  }
  appendOutput(data.output);
  if (data.error) {
    appendOutput([`Error: ${data.error}`]);
  }
  input.disabled = data.sessionEnded;
  if (data.sessionEnded || data.gameWon) {
    sessionId = null;
  }
}

form.addEventListener('submit', async (e) => {
  e.preventDefault();
  const line = input.value;
  input.value = '';
  if (line.trim().length > 0) {
    appendOutput([`> ${line}`]);
  }
  await sendLine(line);
});

bootstrap().catch((err) => {
  appendOutput([String(err)]);
});
