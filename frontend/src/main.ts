import './style.css';

const API_BASE = '/api';
const MASTER_COMBAT_WAIT_MS = 1000;

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
const submitBtn = form.querySelector('button[type="submit"]') as HTMLButtonElement;

let sessionId: string | null = null;
let requestInFlight = false;

function setControlsEnabled(enabled: boolean) {
  input.disabled = !enabled;
  submitBtn.disabled = !enabled;
  if (enabled) {
    input.focus();
  }
}

function applyInputState(data: StepPayload) {
  const canInput = Boolean(data.awaitingInput) && !data.sessionEnded && !data.gameWon;
  setControlsEnabled(canInput && !requestInFlight);
}

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

function delay(ms: number): Promise<void> {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function isAttackLine(line: string): boolean {
  return line.includes(' uses ') && (line.includes(' damage.') || line.includes('misses.') || line.includes('miss.'));
}

async function appendOutputWithCombatPacing(data: StepPayload) {
  const lines = data.output;
  const attackIndexes: number[] = [];
  for (let i = 0; i < lines.length; i++) {
    if (isAttackLine(lines[i])) {
      attackIndexes.push(i);
      if (attackIndexes.length === 2) break;
    }
  }

  const hasCombatMenu =
    lines.some((line) => line.includes('1. Heavy Attack')) &&
    lines.some((line) => line.includes('2. Light Attack')) &&
    lines.some((line) => line.includes('3. Run'));

  // Mirror master pacing in web mode: player attack -> wait -> enemy attack -> wait -> next-round UI.
  if (data.phase === 'playing' && hasCombatMenu && attackIndexes.length === 2) {
    const firstAttack = attackIndexes[0];
    const secondAttack = attackIndexes[1];
    appendOutput(lines.slice(0, firstAttack + 1));
    await delay(MASTER_COMBAT_WAIT_MS);
    appendOutput(lines.slice(firstAttack + 1, secondAttack + 1));
    await delay(MASTER_COMBAT_WAIT_MS);
    appendOutput(lines.slice(secondAttack + 1));
    return;
  }

  appendOutput(lines);
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
  requestInFlight = true;
  setControlsEnabled(false);
  const data = await postJson<StepPayload>(`${API_BASE}/sessions`, {
    contentRoot: '../content',
  });
  requestInFlight = false;
  sessionId = data.sessionId ?? null;
  if (data.clearViewport) {
    logEl.textContent = '';
  }
  await appendOutputWithCombatPacing(data);
  applyInputState(data);
}

async function sendLine(line: string) {
  if (!sessionId) return;
  requestInFlight = true;
  setControlsEnabled(false);
  const data = await postJson<StepPayload>(`${API_BASE}/sessions/${encodeURIComponent(sessionId)}/step`, {
    line,
  });
  requestInFlight = false;
  if (data.clearViewport) {
    logEl.textContent = '';
  }
  await appendOutputWithCombatPacing(data);
  if (data.error) {
    appendOutput([`Error: ${data.error}`]);
  }
  applyInputState(data);
  if (data.sessionEnded || data.gameWon) {
    sessionId = null;
  }
}

form.addEventListener('submit', async (e) => {
  e.preventDefault();
  if (requestInFlight || input.disabled) return;
  const line = input.value;
  input.value = '';
  if (line.trim().length > 0) {
    appendOutput([`> ${line}`]);
  }
  try {
    await sendLine(line);
  } catch (err) {
    requestInFlight = false;
    setControlsEnabled(true);
    appendOutput([String(err)]);
  }
});

bootstrap().catch((err) => {
  appendOutput([String(err)]);
});
