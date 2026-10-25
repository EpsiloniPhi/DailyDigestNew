# Curriculum (Foundations Restart, from 2026-10-26)

All three study tracks restarted from zero on 2026-10-26. The goal is a single staircase: every lesson stands on the ones before it, and nothing is used before it has been explained.

## How the daily run uses this file

- **One lesson per track per day, in order.** The run takes the next lesson in each track (see `index.md`, rows from the Foundations Restart onwards).
- **A lesson may take 2 or 3 days.** If a lesson is too big to explain deeply in one day, split it: write "Lesson 9 (part 1 of 2)" in the index and continue it the next day. Going slowly is fine; skipping steps is not.
- **Look back first.** Every lesson opens with a short "Where we are" box: which earlier lessons it builds on, recapped in two or three sentences.
- **Revision days.** Every **Sunday (Hong Kong time)** is a revision day for all three tracks, with no new lessons:
  - recap the week's lessons in a few lines each, with the one picture to remember;
  - one older lesson revisited ("spaced repetition": pick one from roughly 2 to 6 weeks earlier once there is enough history);
  - fresh practice problems (new ones, never reused from the repo) with full solutions, and for Algorithms a new variant problem with C++ code.
- The run may also add a short revision box on a normal day when today's lesson leans heavily on an old one.
- Difficulty ramps up slowly. Early interview questions are easy warm-ups; they become real interview level as the foundations grow.

## 1. Probability & Statistics (with the maths explained)

**Unit A: Foundations**
1. What probability is: experiments, outcomes, sample space, events; equally likely outcomes; probability as long-run frequency
2. The language of events: "or", "and", "not" (union, intersection, complement); the addition rule; inclusion-exclusion
3. Counting I: the multiplication principle, arrangements (permutations), factorials
4. Counting II: choosing (combinations), $\binom{n}{k}$, Pascal's triangle, cards and committees
5. Counting III: stars and bars, balls into boxes, counting the complement
6. Conditional probability: shrinking the room
7. Independence, and why independent is not the same as "cannot happen together"
8. The law of total probability: splitting into cases, tree diagrams
9. Bayes' rule: counting people, the odds form
10. Classic puzzles with the toolkit: Monty Hall, the birthday problem, boy-or-girl

**Unit B: Discrete random variables**
11. Random variables, distributions (PMF) and the CDF
12. Expected value as a balance point; reading the $\sum$ notation
13. Linearity of expectation and indicator variables
14. Variance and standard deviation; covariance; variance of a sum
15. Bernoulli and binomial distributions
16. Geometric distribution and memorylessness
17. Poisson distribution as the limit of many rare events
18. First-step analysis: expected waiting times with recursion (e.g. flips until HH)

**Unit C: The calculus toolkit (slowly, from scratch)**
19. Sums and series we need: arithmetic and geometric series, the series for $e^x$
20. Integration as adding up thin slices: area under a curve, reading $\int f(x)\,dx$
21. Integration techniques we need: power rule, exponentials, substitution, integration by parts
22. Double integrals as volume, and integrating over regions like $\{x < y\}$

**Unit D: Continuous random variables**
23. Densities: probability as area, PDF vs CDF, the uniform distribution
24. Expectation and variance with integrals
25. The exponential distribution and memorylessness
26. The normal distribution: the bell curve, z-scores, 68-95-99.7
27. Joint distributions and geometric probability (meeting problems, broken stick)
28. Functions and sums of random variables; convolution
29. Conditional expectation and the tower property
30. The law of total variance

**Unit E: The big theorems**
31. Generating functions (PGF, MGF)
32. Inequalities: Markov, Chebyshev, Jensen
33. The law of large numbers
34. The central limit theorem, and when it fails
35. Order statistics

**Unit F: Random processes**
36. Markov chains
37. Random walks and gambler's ruin
38. Martingales and optional stopping
39. Poisson processes and exponential races
40. Brownian motion
41. Ito's lemma
42. Change of measure (intuition)

**Unit G: Statistics for quants**
43. Estimation and maximum likelihood
44. Regression from first principles
45. Hypothesis testing and p-hacking in backtests
46. Covariance matrices and PCA
47. The Kelly criterion

## 2. Derivatives

**Unit A: Money and markets**
1. Interest rates: simple, compound and continuous compounding ($e^{rt}$ explained)
2. Present value and discount factors: money today vs money later
3. How a market works: bid and ask, market makers, long and short, leverage
4. FX basics: currency pairs, base and quote currency, pips, how EURUSD and USDJPY are quoted
5. Forwards: what they are, the payoff, who uses them
6. Pricing a forward by no-arbitrage: cost of carry, FX forwards and covered interest parity, forward points
7. Futures vs forwards: exchanges, margin, daily mark-to-market

**Unit B: Bonds and rates**
8. Bonds: coupons, price and yield, why they move opposite
9. Duration and DV01
10. The yield curve; US Treasuries and how they are quoted
11. Repo: borrowing against bonds
12. Interest rate swaps
13. Treasury futures and cheapest-to-deliver

**Unit C: Options from zero**
14. What an option is: calls and puts, payoff diagrams, premium
15. Buyers and sellers, moneyness, intrinsic value and time value
16. Put-call parity
17. Arbitrage bounds on option prices
18. The one-step binomial model: pricing by replication
19. Multi-step trees and risk-neutral probability
20. Black-Scholes: the formula explained term by term
21. Garman-Kohlhagen: Black-Scholes for FX

**Unit D: Greeks and volatility**
22. Delta and hedging
23. Gamma
24. Theta, and the theta-gamma trade-off
25. Vega and implied volatility
26. Gamma scalping: realized vs implied
27. The volatility smile and skew

**Unit E: Option strategies**
28. Vertical spreads
29. Butterflies and condors
30. Calendar spreads
31. Straddles and strangles
32. Risk reversals and skew trading
33. FX option conventions: ATM delta-neutral straddle, 25-delta risk reversals and butterflies, premium-adjusted delta

**Unit F: Advanced**
34. Variance and volatility swaps
35. Barrier and digital options; pin risk
36. Rates volatility: caps, floors, swaptions
37. Dispersion and correlation trading
38. Convexity adjustments
39. Exotics and structured products (autocallables, TARFs)

## 3. Algorithms & Data Structures (C++)

**Unit A: Foundations**
1. Big-O: counting steps, and why $n^2$ vs $n \log n$ matters
2. C++ refresher: `vector`, `string`, references, functions, loops
3. Arrays and strings: scanning, in-place updates
4. Hash maps and sets (`unordered_map`, `unordered_set`)
5. Prefix sums
6. Sorting: how merge sort works, `std::sort`, custom comparators
7. Two pointers
8. Sliding window
9. Recursion and the call stack
10. Binary search
11. Binary search on the answer

**Unit B: Linear structures**
12. Stacks
13. Queues and deques
14. Monotonic stacks
15. Linked lists

**Unit C: Trees and heaps**
16. Binary trees: recursion on trees, traversals
17. Binary search trees (and `std::set` / `std::map`)
18. Lowest common ancestor
19. Heaps and priority queues

**Unit D: Graphs**
20. Graph representations and BFS
21. DFS and connected components
22. Topological sort
23. Dijkstra's shortest paths
24. Union-Find

**Unit E: Dynamic programming**
25. DP from scratch: Fibonacci, memoization vs tabulation
26. 1D DP
27. 2D grid DP
28. Knapsack
29. Longest increasing subsequence
30. Edit distance and longest common subsequence

**Unit F: Further topics**
31. Backtracking
32. Tries and string algorithms
33. Intervals and sweep line
34. Bit manipulation
35. Fenwick trees and segment trees
36. Design problems: LRU cache, order book, rate limiter
37. Randomized algorithms
38. Concurrency basics in C++
39. Low-latency C++

## 4. Music

Classic rock, one song per day, never repeated. Very famous bands only (Pink Floyd, The Doors, The Beatles, Led Zeppelin, The Rolling Stones, The Who, Queen, Jimi Hendrix, Cream, Eagles, Fleetwood Mac, Dire Straits, Deep Purple, Lynyrd Skynyrd, The Kinks, etc.). Vary the band from day to day.
