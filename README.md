MonteCarloMethod
================

This program finds a mathematical function value very close to the minimum using a Monte Carlo (random sampling) method. Given a large number of random samples for x between -10 <= x <= 10, evaluating the function x^2 at every point, MonteCarloMethod finds the value of x that yields the smallest number for x^2. This program is an example of the producer-consumer problem. The solution implements one binary semaphore and two counting semaphores to allow for synchronization.
