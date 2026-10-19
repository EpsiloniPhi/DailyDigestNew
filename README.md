# DailyDigest

A daily quant / trader interview prep digest, generated every morning at 7:00am Hong Kong time.

**Read the PDF.** Each day has a PDF (`YYYY/MM/YYYY-MM-DD.pdf`) with the formulas, tables and code properly rendered. GitHub's markdown preview does not render all of it.

Each day has four sections:

1. **Probability & Statistics**: one big idea explained intuitively (analogies first, formulas second), plus a quant interview question with a full worked solution.
2. **Derivatives**: one concept or strategy, explained from first principles with analogies and a real-world trade with realistic numbers.
3. **Algorithms & Data Structures**: an interview problem with a first-principles explanation, a commented C++ solution, and complexity analysis.
4. **Music**: one classic rock song with the story behind it and why it is worth hearing.

## Style guide (followed by every daily run)

- **Make hard things simple, do not pick simple things.** Topics follow CURRICULUM.md and stay interview-level, but every explanation starts from first principles with an everyday analogy or picture, then a small example, and only then the formula. Plain, simple English. Short sentences.
- **Length.** Sections 1 and 2: about 900 to 1,100 words each. Pick fewer ideas and go deep on one; leftover material can wait for another day. Section 3 can stay at its current length (about 2,000 words including code). Section 4: about 300 to 500 words. Total reading time about 15 minutes.
- **Probability:** one core idea, an analogy that makes it feel like common sense, a worked mini example, the interview question, and a solution that counts concrete cases (e.g. "imagine 1,000 traders") before using algebra, plus a sanity check.
- **Derivatives:** a real-world scenario (who is on each side and why), analogies for each mechanic, one worked trade with realistic numbers, one real market episode where it went wrong if relevant, and 2 to 3 interview questions with short answers.
- **Music:** classic rock only for now (Pink Floyd, The Doors, The Beatles, Led Zeppelin, The Rolling Stones, The Who, Queen, etc.). Very famous bands, not new ones. One song per day, never repeated (see the Music column in `index.md`). Give the context and story, why people love it, and what to listen for. Do not quote lyrics.
- No em dashes anywhere.

## Layout

- `YYYY/MM/YYYY-MM-DD.md`: the day's digest (source)
- `YYYY/MM/YYYY-MM-DD.pdf`: the day's digest as a PDF (read this one)
- `YYYY/MM/YYYY-MM-DD.cpp`: the day's C++ solution (compile with `g++ -std=c++17 -O2 file.cpp`)
- `index.md`: running log of every topic covered, used to avoid repeats and keep a progression
- `CURRICULUM.md`: the planned progression for each track
- `tools/make_pdf.py`: renders a digest to PDF (`python3 tools/make_pdf.py 2026/10/2026-10-19.md`). Needs pandoc, Python Playwright with Chromium, and npm (KaTeX is installed into `tools/node_modules` on first use).
