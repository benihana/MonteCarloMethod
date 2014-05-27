MonteCarloMethod
================

This program finds a mathematical function value very close to the minimum using a Monte Carlo (random sampling) method. Given a large number of random samples for x between -10 <= x <= 10, evaluating the function x^2 at every point, MonteCarloMethod finds the value of x that yields the smallest number for x^2. This program is a prime example of the producer-consumer problem. This solution implements one binary sempahore and two counting sempahores to allow for smooth synchronization.
