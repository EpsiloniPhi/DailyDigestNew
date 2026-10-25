# DailyDigest

A daily quant / trader interview prep digest, generated every morning at 7:00am Hong Kong time.

**Read the PDF.** Each day has a PDF (`YYYY/MM/YYYY-MM-DD.pdf`) with the formulas, tables and code properly rendered. GitHub's markdown preview does not render all of it.

Each day has four sections:

1. **Probability & Statistics**: one lesson, built from the basics, with every formula explained, plus self-check questions and an interview-style question with a full worked solution.
2. **Derivatives**: one lesson, built from the basics, with analogies and a worked example with realistic numbers.
3. **Algorithms & Data Structures**: one lesson and problem, a first-principles explanation, a commented C++ solution, and complexity analysis.
4. **Music**: one classic rock song with the story behind it and why it is worth hearing.

## Style guide (followed by every daily run)

Since 2026-10-26 all three study tracks follow the **Foundations Restart** in `CURRICULUM.md`: they start from zero and build one lesson on top of the last, with revision days every Sunday.

- **Build from the ground up.** Never use an idea that has not been taught in an earlier lesson. Each lesson opens with a short "Where we are" box that recaps the earlier lessons it builds on.
- **Deep analogies, not one-liners.** Pick one central analogy per section and carry it all the way through. Spell out what each part of the analogy stands for (a small table "in the picture / in the maths" works well). Walk through a concrete scene with it. Say where the analogy stops working.
- **Explain every formula.** Before a formula: the idea in words. After it: what each symbol means, how to read it out loud in plain English, and the same formula with real numbers plugged in. Never skip algebra steps. Integrals, sums and derivatives are explained step by step ("an integral is adding up thin slices"), assuming nothing beyond what earlier lessons covered.
- **Concrete before abstract.** Count cases with real numbers ("imagine 1,000 traders") before any algebra. Small example first, general rule second.
- **Plain English, short sentences.** One idea at a time. It is fine for a lesson to span 2 or 3 days.
- **Check yourself.** End each study section with 2 or 3 quick self-check questions (answers at the bottom of the section), then an interview-style question at the right level for the lesson, with a full step-by-step solution and a sanity check.
- **Length.** About 25 minutes of reading in total: Probability about 1,500 to 1,800 words; Derivatives about 1,500 to 1,800 words; Algorithms about 2,000 to 2,500 words including code; Music about 300 to 500 words.
- **Derivatives:** start with a real-world scene (who does it and why), analogies for each mechanic, one worked example with realistic numbers (FX examples preferred).
- **Algorithms:** one problem at the level of the lesson, brute force first, then the insight, complexity explained, the C++ solution, and a line-by-line walkthrough of the C++ itself.
- **Music:** classic rock only (Pink Floyd, The Doors, The Beatles, Led Zeppelin, The Rolling Stones, The Who, Queen, etc.). Very famous bands, not new ones. One song per day, never repeated (see the Music column in `index.md`). Give the context and story, why people love it, and what to listen for. Do not quote lyrics.
- No em dashes anywhere.

## Layout

- `YYYY/MM/YYYY-MM-DD.md`: the day's digest (source)
- `YYYY/MM/YYYY-MM-DD.pdf`: the day's digest as a PDF (read this one)
- `YYYY/MM/YYYY-MM-DD.cpp`: the day's C++ solution (compile with `g++ -std=c++17 -O2 file.cpp`)
- `index.md`: running log of every topic covered, used to avoid repeats and keep a progression
- `CURRICULUM.md`: the planned progression for each track
- `tools/make_pdf.py`: renders a digest to PDF (`python3 tools/make_pdf.py 2026/10/2026-10-19.md`). Needs pandoc, Python Playwright with Chromium, and npm (KaTeX is installed into `tools/node_modules` on first use).
