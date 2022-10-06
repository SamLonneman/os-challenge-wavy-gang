# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 1: Multithreading
**Background**: The goal of this experiment is to determine whether multithreading is faster than using separate processes for each request. Our first working implementation for the milestone can be found on a branch called [milestone](https://github.com/SamLonneman/os-challenge-wavy-gang/tree/milestone). This implementation will serve as the control in this experiment.

**Hypothesis**: The implementation which uses one thread per request will score higher than the implementation which uses one process per request. We hypothesize this due to the assumption that creating a new thread is computationally easier than starting a new process because threads share a process' memory, so less memory will be copied at each step.

**Variables**: The independent variable in this experiment is whether processes or threads are used to process each request. The dependent variable will be the resulting score returned by the client using the milestone run configuration. In both cases, we will use a similar processing structure in which the parent thread/process accepts connections from the client and produces a child thread/process for each request to process and return a response. Furthermore, when testing performance of each method, an identical run configuration without any randomization will be used. All tests will be run on the same virtual environment running on the same physical computer.
