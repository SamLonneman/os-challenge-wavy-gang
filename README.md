# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 6: 4 Processes Each With Multithreading

### Background 
We are taking advantage of the knowledge that there are four cores to optimize by running 4 threads concurrently. 
This builds on experiments one and two where we test the speed of multithreading vs multiple processes and running four processes at once. 

### Hypothesis 
Due to experiment one indicating that multithreading was not faster than running multiple processes 
and experiment 2 proving that running four processes at once was faster than one at a time, it can be infered that
this will have better performance that multithreading alone but decreased performance when compared to just running the
four processes without the addition of multithreading on each one. 