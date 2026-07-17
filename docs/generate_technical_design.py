#!/usr/bin/env python3
"""Generate TECHNICAL_DESIGN.html with all SVG diagrams."""

def main():
    html = []
    
    # CSS
    html.append("""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>HS Edit — Technical Design</title>
<style>
:root{--bg:#1a1b26;--surface:#24283b;--surface2:#2f3347;--text:#c0caf5;--text-dim:#565f89;--accent:#7aa2f7;--accent2:#bb9af7;--green:#9ece6a;--orange:#ff9e64;--red:#f7768e;--cyan:#7dcfff;--yellow:#e0af68;--border:#3b4261;--code-bg:#16161e}
*{margin:0;padding:0;box-sizing:border-box}
body{font-family:'JetBrains Mono','Fira Code','SF Mono',Consolas,monospace;background:var(--bg);color:var(--text);line-height:1.7;font-size:15px;padding:40px 48px;max-width:900px;margin:0 auto}
h1{font-size:32px;color:var(--accent);margin-bottom:8px}
h1 .subtitle{font-size:16px;color:var(--text-dim);display:block;margin-top:8px;font-weight:normal}
h2{font-size:24px;color:var(--accent2);margin-top:48px;margin-bottom:16px;padding-bottom:8px;border-bottom:1px solid var(--border)}
h3{font-size:18px;color:var(--cyan);margin-top:32px;margin-bottom:12px}
h4{font-size:15px;color:var(--yellow);margin-top:24px;margin-bottom:8px;text-transform:uppercase;letter-spacing:1px}
p{margin-bottom:16px}
ul,ol{margin:12px 0 16px 24px}
li{margin-bottom:6px}
pre{background:var(--code-bg);border:1px solid var(--border);border-radius:8px;padding:16px 20px;overflow-x:auto;margin:16px 0 24px;font-size:13.5px;line-height:1.6}
code{font-family:inherit}
pre code{color:var(--text)}
.kw{color:var(--accent2)}.fn{color:var(--accent)}.type{color:var(--cyan)}.str{color:var(--green)}.cmt{color:var(--text-dim);font-style:italic}.num{color:var(--orange)}.inc{color:var(--yellow)}.macro{color:var(--orange)}.punct{color:var(--text-dim)}
.box{border-left:4px solid;padding:16px 20px;margin:20px 0;border-radius:0 8px 8px 0}
.box-info{border-color:var(--accent);background:rgba(122,162,247,.08)}.box-tip{border-color:var(--green);background:rgba(158,206,106,.08)}.box-warn{border-color:var(--orange);background:rgba(255,158,100,.08)}.box-danger{border-color:var(--red);background:rgba(247,118,142,.08)}
.box-title{font-weight:bold;font-size:13px;text-transform:uppercase;letter-spacing:1px;margin-bottom:8px}
.box-info .box-title{color:var(--accent)}.box-tip .box-title{color:var(--green)}.box-warn .box-title{color:var(--orange)}.box-danger .box-title{color:var(--red)}
table{width:100%;border-collapse:collapse;margin:16px 0 24px;font-size:14px}
th{text-align:left;color:var(--accent);font-weight:600;padding:10px 16px;border-bottom:2px solid var(--accent);font-size:13px;text-transform:uppercase;letter-spacing:1px}
td{padding:10px 16px;border-bottom:1px solid var(--border)}
td code{background:var(--surface2);padding:2px 6px;border-radius:4px;font-size:13px;color:var(--cyan)}
.diagram{background:var(--code-bg);border:1px solid var(--border);border-radius:8px;padding:20px;margin:20px 0;text-align:center}
.diagram svg{max-width:100%;height:auto}
.diagram .box-shape{fill:var(--surface2);stroke:var(--accent);stroke-width:1.5}
.diagram .box-shape-accent{fill:var(--surface);stroke:var(--accent2);stroke-width:1.5}
.diagram .box-shape-green{fill:var(--surface2);stroke:var(--green);stroke-width:1.5}
.diagram .box-text{fill:var(--text);font-size:12px;font-family:monospace}
.diagram .box-text-accent{fill:var(--accent);font-size:12px;font-family:monospace}
.diagram .arrow-line{stroke:var(--text-dim);stroke-width:1.5;fill:none}
.diagram .arrow-head{fill:var(--text-dim)}
.diagram .label{fill:var(--accent);font-weight:bold}
.diagram .coord-text{fill:var(--text-dim);font-size:11px;font-family:monospace}
.diagram .axis-label{fill:var(--accent);font-size:13px;font-family:monospace}
.diagram .origin-text{fill:var(--accent);font-size:12px;font-family:monospace;font-weight:bold}
.diagram .page-shape{fill:var(--surface2);stroke:var(--cyan);stroke-width:1.5}
.diagram .page-split{fill:var(--surface);stroke:var(--green);stroke-width:1.5}
.diagram .line-text{fill:var(--text);font-size:11px;font-family:monospace}
.diagram .split-line{stroke:var(--green);stroke-width:2;stroke-dasharray:5,5}
.diagram .buffer-shape{fill:var(--surface);stroke:var(--orange);stroke-width:2}
.diagram .window-shape{fill:var(--surface2);stroke:var(--cyan);stroke-width:1.5}
.diagram .ref-line{stroke:var(--text-dim);stroke-width:1;stroke-dasharray:3,3}
.diagram .config-shape{fill:var(--surface2);stroke:var(--yellow);stroke-width:1.5}
.diagram .module-shape{fill:var(--surface);stroke:var(--accent2);stroke-width:1.5}
.diagram .discover-arrow{stroke:var(--green);stroke-width:2}
.diagram .execute-arrow{stroke:var(--orange);stroke-width:2}
hr{border:none;border-top:1px solid var(--border);margin:48px 0}
</style>
</head>
<body>
<h1 class="anchor" id="top">HS Edit — Technical Design</h1>
<span class="subtitle">Modular TUI Text Editor Architecture</span>""")
    
    # Overview
    html.append("""
<section id="overview">
<h2>Overview</h2>
<p>HS Edit is a modular TUI text editor built with C++ and ncurses. The editor follows a microservices architecture where core functionality is handled by the base editor, and features are implemented as independent executables (modules) that communicate via standard I/O.</p>
</section>
<hr>""")
    
    # Architecture
    html.append("""
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
  <li><strong>Input Handler</strong> — Receives keystrokes from ncurses, dispatches to router</li>
  <li><strong>Render Engine</strong> — Handles ncurses rendering (windows, buffers, status bar)</li>
  <li><strong>Router</strong> — Command dispatcher that determines which module handles each command</li>
  <li><strong>Module Registry</strong> — Tracks registered modules and their capabilities</li>
</ul>
</section>
<hr>""")
    
    # Module System
    html.append("""
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
  <text x="350" y="158" text-anchor="middle" class="coord-text">Command Invoked → Spawn → Respond → Close</text>
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
<hr>""")
    
    # Text Representation
    html.append("""
<section id="text-representation">
<h2>Text Representation: Paged Text Buffer Model</h2>
<p>Text is stored in <strong>TextWindowBuffer</strong>, a dynamic buffer containing a vector of UTF-8 lines. This serves as the foundation for both small and large file handling.</p>

<h3>TextWindowBuffer</h3>
<pre><code><span class="kw">class</span> <span class="type">TextWindowBuffer</span> <span class="punct">{</span>
    <span class="kw">using</span> TLine <span class="op">=</span> std::u8string<span class="punct">;</span>           <span class="cmt">// UTF-8 encoded line</span>
    <span class="kw">using</span> TLines <span class="op">=</span> std::vector&lt;TLine&gt;<span class="punct">;</span>
    <span class="kw">using</span> TLineIndex <span class="op">=</span> size_t<span class="punct">;</span>
    
    TLineIndex startLine<span class="punct">;</span>
    TLineIndex hintLineWidth<span class="punct">;</span>
    TLines lines<span class="punct">;</span>
    
<span class="kw">public</span><span class="punct">:</span>
    TLine&amp; AddLine<span class="punct">();</span>                      <span class="cmt">// Add empty line</span>
    <span class="kw">void</span> AddLine<span class="punct">(</span><span class="kw">const</span> TLine&amp; line<span class="punct">);</span>
    <span class="kw">void</span> EmplaceLine<span class="punct">(</span>TLine&amp;&amp; line<span class="punct">);</span>
    TLine ExtractLine<span class="punct">(</span>TLineIndex<span class="punct">);</span>
    <span class="kw">void</span> RemoveLine<span class="punct">(</span>TLineIndex<span class="punct">);</span>
    TextWindowBuffer Split<span class="punct">(</span>TLineIndex<span class="punct">);</span>
<span class="punct">};</span></code></pre>

<h3>Unicode Support</h3>
<div class="box box-info">
<div class="box-title">UTF-8 Native</div>
<p>HS Edit uses <strong>UTF-8 encoding</strong> throughout the text buffer for proper Unicode support:</p>
<ul>
  <li><strong>std::u8string</strong>: All lines stored as UTF-8 encoded strings</li>
  <li><strong>Native Unicode</strong>: Supports CJK, emoji, RTL scripts</li>
  <li><strong>Efficient Processing</strong>: Line operations work on byte sequences</li>
  <li><strong>Encoding Preservation</strong>: File I/O preserves UTF-8 without conversion</li>
</ul>
</div>

<h3>Line Splitting Behavior</h3>
<p>All line insertion operations automatically split input by newline characters:</p>
<pre><code><span class="cmt">// Input with embedded newlines is split into multiple lines</span>
buffer<span class="punct">.</span>AddLine<span class="punct">(</span>u8<span class="str">"line1\nline2\nline3"</span><span class="punct">);</span>
<span class="cmt">// Results in: ["line1", "line2", "line3"]</span>

buffer<span class="punct">.</span>InsertLine<span class="punct">(</span><span class="num">0</span><span class="punct">,</span> u8<span class="str">"\n"</span><span class="punct">);</span>  <span class="cmt">// Inserts empty line at position 0</span>
buffer<span class="punct">.</span>ReplaceLine<span class="punct">(</span><span class="num">5</span><span class="punct">,</span> u8<span class="str">"old\nnew"</span><span class="punct">);</span>  <span class="cmt">// Replaces line 5 with two lines</span></code></pre>

<h3>Performance Characteristics</h3>
<table>
  <thead>
    <tr>
      <th>Operation</th>
      <th>Complexity</th>
      <th>Notes</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><code>AddLine</code>, <code>EmplaceLine</code></td>
      <td>O(1)</td>
      <td>Amortized constant time</td>
    </tr>
    <tr>
      <td><code>InsertLine</code>, <code>ReplaceLine</code></td>
      <td>O(n)</td>
      <td>Where n is lines after position</td>
    </tr>
    <tr>
      <td><code>ExtractLine</code>, <code>RemoveLine</code></td>
      <td>O(n)</td>
      <td>Due to vector shift</td>
    </tr>
    <tr>
      <td><code>NumLines</code>, <code>GetBuffer</code></td>
      <td>O(1)</td>
      <td>Direct access</td>
    </tr>
  </tbody>
</table>

<div class="box box-tip">
<div class="box-title">Move Semantics</div>
<p>Split, ExtractLine use move semantics for efficient transfer of ownership without copying data.</p>
</div>

<h3>Paged Text Handling (Future Extension)</h3>
<p>For large files, TextWindowBuffer can be extended with page-based management:</p>
<pre><code>TextWindowBuffer<span class="punct">:</span> <span class="punct">[</span><span class="num">0</span><span class="punct">-</span><span class="num">2000</span><span class="punct">]</span> <span class="punct">(</span><span class="num">2000</span> lines<span class="punct">)</span>
<span class="cmt">// Split at line 1000</span>
TextWindowBuffer<span class="punct">:</span> <span class="punct">[</span><span class="num">0</span><span class="punct">-</span><span class="num">1000</span><span class="punct">]</span> <span class="punct">(</span><span class="num">1000</span> lines<span class="punct">)</span>
TextWindowBuffer<span class="punct">:</span> <span class="punct">[</span><span class="num">1000</span><span class="punct">-</span><span class="num">2000</span><span class="punct">]</span> <span class="punct">(</span><span class="num">1000</span> lines<span class="punct">)</span></code></pre>

<div class="box box-warn">
<div class="box-title">Split Threshold</div>
<p>Configurable (default: 1000 lines). Pages can be cached to disk for very large files.</p>
</div>
</section>
<hr>""")
    
    # Write to file
    with open('TECHNICAL_DESIGN.html', 'w', encoding='utf-8') as f:
        f.write('\n'.join(html))
    
    print("Generated TECHNICAL_DESIGN.html")

if __name__ == '__main__':
    main()
