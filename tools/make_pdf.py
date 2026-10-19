#!/usr/bin/env python3
"""Render a digest markdown file to PDF.

Usage: python3 tools/make_pdf.py 2026/10/2026-10-19.md [more.md ...]
Writes the PDF next to each markdown file (same name, .pdf extension).
Pipeline: pandoc (markdown -> standalone HTML) -> KaTeX renders the math in headless
Chromium -> Chromium prints the page to PDF. KaTeX is installed into tools/node_modules
with npm on first use.
"""
import pathlib
import subprocess
import sys
import tempfile

from playwright.sync_api import sync_playwright

HERE = pathlib.Path(__file__).resolve().parent
CSS = HERE / "digest.css"
KATEX = HERE / "node_modules" / "katex" / "dist"


def ensure_katex() -> None:
    if not (KATEX / "katex.min.js").exists():
        subprocess.run(["npm", "install", "--no-audit", "--no-fund", "--silent"], cwd=HERE, check=True)


def md_to_html(md_path: pathlib.Path, html_path: pathlib.Path) -> None:
    subprocess.run(
        [
            "pandoc", str(md_path),
            "-f", "markdown+tex_math_dollars+pipe_tables+backtick_code_blocks+hard_line_breaks",
            "-t", "html5", "-s", f"--katex={KATEX.as_uri()}/",
            "--highlight-style=tango",
            "--css", CSS.as_uri(),
            "--metadata", f"title={md_path.stem}",
            "-o", str(html_path),
        ],
        check=True,
    )


def main(paths):
    ensure_katex()
    with sync_playwright() as pw:
        browser = pw.chromium.launch()
        page = browser.new_page()
        for p in paths:
            md = pathlib.Path(p).resolve()
            pdf = md.with_suffix(".pdf")
            with tempfile.TemporaryDirectory() as tmp:
                html = pathlib.Path(tmp) / (md.stem + ".html")
                md_to_html(md, html)
                page.goto(html.as_uri())
                page.wait_for_load_state("networkidle")
                page.wait_for_function("document.fonts.status === 'loaded'")
                errors = page.evaluate("document.querySelectorAll('.katex-error').length")
                if errors:
                    print(f"warning: {errors} KaTeX error(s) in {md.name}", file=sys.stderr)
                page.pdf(
                    path=str(pdf), format="A4", print_background=True,
                    display_header_footer=True,
                    header_template="<span></span>",
                    footer_template=(
                        '<div style="font-size:8px;width:100%;text-align:center;color:#888;">'
                        f'Daily Quant Digest {md.stem} &middot; page <span class="pageNumber"></span>'
                        ' of <span class="totalPages"></span></div>'
                    ),
                    margin={"top": "16mm", "bottom": "18mm", "left": "15mm", "right": "15mm"},
                )
            print(f"wrote {pdf}")
        browser.close()


if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    main(sys.argv[1:])
