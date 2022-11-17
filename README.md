# 02159 Operating Systems Challenge
### Wavy Gang - Sam Lonneman, Alana Vanzo, & Pedro Wachtendorff

## Experiment 6.1: Priority Over all Other Factors

### Background
This experiment purely priorities the priority level of each request as it will output the requests directly in order from highest priority to lowest priority
(regardless of when the request is received). It makes use of 4 processes to optimize for the four core system, as each process will begin ordering requests
based on priority level and processing them once all requests have been received in sync. 

### Hypothesis 
This test will likely yield a higher score than experiment 6.3 as although the requests will be outputted in descending
order from highest priorities, on average request time from start to finish for each request will take much longer due 
to every request having to wait for all requests to be loaded in and saved. 

### Purpose 
The purpose of this experiment is to isolate the importance of priority and test performance when priority is the most valued 
variable. The program is running with multiple processes as well in an attempt to optimize score where possible while not 
taking any focus away from priority level.

### Run Configuration
SERVER=192.168.101.10 
\
PORT=5003
\
SEED=3435245
\
TOTAL=100
\
START=1
\
DIFFICULTY=300000
\
REP_PROB_PERCENT=20
\
DELAY_US=10
\
PRIO_LAMBDA=0.2   

Note that a different client run configuration has been used as to test the priorities in an enviornment 
where most priorities are obtained. As storing the values of each request takes time, theoretically if all requests were to 
have the same priority then there is no point in taking the time to store values and order. 

### Results
|   Run   | Experiment 6.1 | Experiment 6.3 | Experiment 1 (multithreading) | Milestone (4 Processes) |
|:-------:|:--------------:|:--------------:|:-----------------------------:|:-----------------------------:|
|    1    |    20531239    |    13981242    |           13059565            |       12125862            |
|    2    |    24377169    |    11725117    |           13776204            |       13490430            |
|    3    |    25979897    |    14212356    |           13418582            |       13636356            |
|    4    |    26949927    |    12156278    |           14793607            |       13897344            |
|    5    |    26348427    |    13715916    |           13273441            |       14376678            |
|    6    |    26309607    |  13395847      |           13675835            |       13279553            |
|    7    |    26522276    |    13546126    |           14633293            |       14786374            |
|    8    |    27009276    |    12068886    |           13387746            |       13800504            |
|    9    |    27395650    |    13875488    |           13774564            |       13497668            |
|   10    |    25919462    |    13944864    |           13070934            |       14355790            |
| Average |    26428980    |    13770646    |           13895749            |       14237429            |

### Conclusion

### Discussion

### Improvements
The evident weakness in this experiment resides in the delay between receiving the first priority and sending requests back,
as the processes will not send a request back until all requests have been received and ordered. Therefore, while the time taken 
for the whole program to run will be very similar to a four process system without priority ordering, the time delay will be very high
for low priority requests received early on as they will then be processed last. 
