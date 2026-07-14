#!/usr/bin/env python3
import sys

css = """
<style>
:root{--bg:#1a1b26;--surface:#24283b;--surface2:#2f3347;--text:#c0caf5;--text-dim:#565f89;--accent:#7aa2f7;--accent2:#bb9af7;--green:#9ece6a;--orange:#ff9e64;--red:#f7768e;--cyan:#7dcfff;--yellow:#e0af68;--border:#3b4261;--code-bg:#16161e}
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:'JetBrains Mono','Fira Code','SF Mono',Consolas,monospace;background:var(--bg);color:var(--text);line-height:1.7;font-size:15px;padding:40px 48px;max-width:900px;margin:0 auto}
h1{font-size:32px;color:var(--accent);margin-bottom:8px}
h1 .subtitle{font-size:16px;color:var(--text-dim);display:block;margin-top:8px;font-weight:normal}
h2{font-size:24px;color:var(--accent2);margin-top:48px;margin-bottom:16px;padding-bottom:8px;border-bottom:1px solid var(--border)}
h3{font-size:18px;color:var(--cyan);margin-top:32px;margin-bottom:12px}
p{margin-bottom:16px}
ul{margin:12px 0 16px 24px}
li{margin-bottom:6px}
pre{background:var(--code-bg);border:1px solid var(--border);border-radius:8px;padding:16px 20px;overflow-x:auto;margin:16px 0 24px;font-size:13.5px;line-height:1.6}
code{font-family:inherit}
pre code{color:var(--text)}
.kw{color:var(--accent2)}.fn{color:var(--accent)}.type{color:var(--cyan)}.str{color:var(--green)}.cmt{color:var(--text-dim);font-style:italic}.num{color:var(--orange)}.inc{color:var(--yellow)}.punct{color:var(--text-dim)}
.diagram{background:var(--code-bg);border:1px solid var(--border);border-radius:8px;padding:20px;margin:20px 0;text-align:center}
.diagram svg{max-width:100%;height:auto}
.diagram .label{fill:var(--accent);font-weight:bold}
.diagram .box-shape{fill:var(--surface2);stroke:var(--accent);stroke-width:1.5}
.diagram .box-shape-accent{fill:var(--surface);stroke:var(--accent2);stroke-width:1.5}
.diagram .box-shape-green{fill:var(--surface2);stroke:var(--green);stroke-width:1.5}
.diagram .box-text{fill:var(--text);font-size:12px;font-family:monospace}
.diagram .box-text-accent{fill:var(--accent);font-size:12px;font-family:monospace}
.diagram .arrow-line{stroke:var(--text-dim);stroke-width:1.5;fill:none}
.diagram .coord-text{fill:var(--text-dim);font-size:11px;font-family:monospace}
.diagram .line-text{fill:var(--text);font-size:11px;font-family:monospace}
.diagram .split-line{stroke:var(--green);stroke-width:2;stroke-dasharray:5,5}
.diagram .page-shape{fill:var(--surface2);stroke:var(--cyan);stroke-width:1.5}
.diagram .page-split{fill:var(--surface);stroke:var(--green);stroke-width:1.5}
.diagram .module-shape{fill:var(--surface);stroke:var(--accent2);stroke-width:1.5}
.diagram .discover-arrow{stroke:var(--green);stroke-width:2}
.diagram .execute-arrow{stroke:var(--orange);stroke-width:2}
hr{border:none;border-top:1px solid var(--border);margin:48px 0}
</style>
"""

sections = []

# Architecture diagram
sections.append('''
<section id="architecture">
<h2>Architecture</h2>
<div class="diagram">
<svg width="600" height="280" viewBox="0 0 600 280">
  <text x="300" y="18" text-anchor="middle" class="label">HS Edit Architecture</text>
  <rect x="150" y="30" width="300" height="60" rx="8" class="box-shape"/>
  <text x="300" y="52" text-anchor="middle" class="box-text-accent">Core Editor</text>
  <text x="300" y="68" text-anchor="middle" class="coord-text">Input | Render | Router</text>
  <rect x="200" y="110" width="200" height="40" rx="6" class="box-shape-accent"/>
  <text x="300" y="135" text-anchor="middle" class="box-text">Module Registry</text>
  <line x1="300" y1="90" x2="300" y2="110" class="arrow-line"/>
  <rect x="50" y="180" width="120" height="40" rx="6" class="module-shape"/>
  <text x="110" y="205" text-anchor="middle" class="box-text">Syntax</text>
  <rect x="240" y="180" width="120" height="40" rx="6" class="module-shape"/>
  <text x="300" y="205" text-anchor="middle" class="box-text">File Manager</text>
  <rect x="430" y="180" width="120" height="40" rx="6" class="module-shape"/>
  <text x="490" y="205" text-anchor="middle" class="box-text">Command Mode</text>
  <line x1="250" y1="150" x2="110" y2="180" class="arrow-line"/>
  <line x1="300" y1="150" x2="300" y2="180" class="arrow-line"/>
  <line x1="350" y1="150" x2="490" y2="180" class="arrow-line"/>
  <text x="300" y="170" text-anchor="middle" class="coord-text">IPC (standard I/O)</text>
</svg>
</div>
<h3>Core Components</h3>
<ul>
  <li><strong>Input Handler</strong> — Receives keystrokes from ncurses</li>
  <li><strong>Render Engine</strong> — Handles ncurses rendering</li>
  <li><strong>Router</strong> — Command dispatcher</li>
  <li><strong>Module Registry</strong> — Tracks registered modules</li>
</ul>
</section>
<hr>
''')

# Module lifecycle
sections.append('''
<section id="module-system">
<h2>Module System</h2>
<h3>Module Lifecycle</h3>
<div class="diagram">
<svg width="700" height="200" viewBox="0 0 700 200">
  <text x="350" y="18" text-anchor="middle" class="label">Module Lifecycle</text>
  <rect x="20" y="40" width="140" height="40" rx="6" class="box-shape"/>
  <text x="90" y="65" text-anchor="middle" class="box-text-accent">1. Scan modules/</text>
  <rect x="210" y="40" width="140" height="40" rx="6" class="box-shape-accent"/>
  <text x="280" y="65" text-anchor="middle" class="box-text">2. Run --reveal</text>
  <rect x="400" y="40" width="140" height="40" rx="6" class="box-shape"/>
  <text x="470" y="65" text-anchor="middle" class="box-text-accent">3. Register</text>
  <rect x="590" y="40" width="100" height="40" rx="6" class="box-shape-accent"/>
  <text x="640" y="65" text-anchor="middle" class="box-text">4. Execute</text>
  <line x1="160" y1="60" x2="210" y2="60" class="discover-arrow"/>
  <line x1="350" y1="60" x2="400" y2="60" class="discover-arrow"/>
  <line x1="540" y1="60" x2="590" y2="60" class="discover-arrow"/>
  <rect x="250" y="120" width="200" height="50" rx="8" class="box-shape-green"/>
  <text x="350" y="142" text-anchor="middle" class="box-text" fill="var(--green)">Module Spawns On-Demand</text>
  <text x="350" y="158" text-anchor="middle" class="coord-text">Command → Spawn → Respond → Close</text>
  <line x1="350" y1="80" x2="350" y2="120" class="execute-arrow"/>
</svg>
</div>
<h3>Protocol Example</h3>
<pre><code><span class="cmt">// Registration (--reveal)</span>
<span class="str">[call] register: name="syntax-highlight", version="1.0"</span>
<span class="str">[call] register_key: key="g g", command="NAVIGATE_TO_TOP"</span>
<span class="cmt">// Command execution</span>
<span class="str">NAVIGATE_TO_TOP</span>
<span class="str">[call] move_cursor: 0, 0</span>
<span class="str">DONE</span></code></pre>
</section>
<hr>
''')

# Text representation
sections.append('''
<section id="text-representation">
<h2>Text Representation: Page-Based Model</h2>
<p>Text is stored in <strong>pages</strong>, which are dynamic buffers containing a vector of lines. Pages split when they grow too large.</p>
<h3>Page Structure</h3>
<pre><code><span class="kw">struct</span> <span class="type">Page</span> <span class="punct">{</span>
    std::vector&lt;std::string&gt; lines<span class="punct">;</span>
    size_t page_id<span class="punct">;</span>
    size_t start_line<span class="punct">;</span>
    size_t end_line<span class="punct">;</span>
    bool modified<span class="punct">;</span>
<span class="punct">};</span></code></pre>
<h3>Page Splitting</h3>
<div class="diagram">
<svg width="500" height="200" viewBox="0 0 500 200">
  <text x="250" y="18" text-anchor="middle" class="label">Page Splitting</text>
  <rect x="30" y="40" width="200" height="60" rx="6" class="page-shape"/>
  <text x="130" y="62" text-anchor="middle" class="line-text">Page 2</text>
  <text x="130" y="78" text-anchor="middle" class="coord-text">Lines 100-300 (200 lines)</text>
  <text x="130" y="90" text-anchor="middle" class="coord-text" fill="var(--orange)">Too large!</text>
  <line x1="230" y1="70" x2="270" y2="70" class="split-line"/>
  <text x="250" y="60" text-anchor="middle" class="coord-text">split</text>
  <rect x="300" y="30" width="160" height="45" rx="6" class="page-split"/>
  <text x="380" y="52" text-anchor="middle" class="line-text">Page 2a</text>
  <text x="380" y="68" text-anchor="middle" class="coord-text">Lines 100-175</text>
  <rect x="300" y="105" width="160" height="45" rx="6" class="page-split"/>
  <text x="380" y="127" text-anchor="middle" class="line-text">Page 2b</text>
  <text x="380" y="143" text-anchor="middle" class="coord-text">Lines 176-300</text>
</svg>
</div>
</section>
<hr>
''')

# Window-Buffer model
sections.append('''
<section id="window-buffer">
<h2>Window/Buffer Model</h2>
<div class="diagram">
<svg width="600" height="250" viewBox="0 0 600 250">
  <text x="300" y="18" text-anchor="middle" class="label">Window/Buffer Relationship</text>
  <rect x="200" y="40" width="200" height="50" rx="6" class="buffer-shape"/>
  <text x="300" y="62" text-anchor="middle" class="box-text-accent">Buffer 1 (file.cpp)</text>
  <text x="300" y="78" text-anchor="middle" class="coord-text">page_table + page cache</text>
  <rect x="50" y="130" width="130" height="50" rx="6" class="window-shape"/>
  <text x="115" y="152" text-anchor="middle" class="box-text">Window A</text>
  <text x="115" y="168" text-anchor="middle" class="coord-text">scroll: 0</text>
  <rect x="235" y="130" width="130" height="50" rx="6" class="window-shape"/>
  <text x="300" y="152" text-anchor="middle" class="box-text">Window B</text>
  <text x="300" y="168" text-anchor="middle" class="coord-text">scroll: 50</text>
  <rect x="420" y="130" width="130" height="50" rx="6" class="window-shape"/>
  <text x="485" y="152" text-anchor="middle" class="box-text">Window C</text>
  <text x="485" y="168" text-anchor="middle" class="coord-text">Buffer 2</text>
  <line x1="115" y1="130" x2="280" y2="90" class="ref-line"/>
  <line x1="300" y1="130" x2="300" y2="90" class="ref-line"/>
  <line x1="485" y1="130" x2="340" y2="90" class="ref-line"/>
