# Thread-Management-Application

**In this application, I focused on thread management (synchronization problem) in Linux operatin systems. Our threads are covid-19 test units and patients.** </br>
**Semaphores are used in application to manage threads.**
</br>
</br>
</br>
#### Compile 
```gcc threadSync.c –o outputfile –lpthread```
</br>
</br>
</br>
### “COVID-19 Test Unit” application explanation
</br>
Today, the units that make Covid-19 tests in medical centers have a very important and critical role.
With the increase in cases and patients, this may cause the density in these units after fillation studies.
Before the test is applied, people are waiting for a long time at the entrance of the hospitals. Because
there is a limited number of test units and healthcare staff. I am launching the “Covid-19 Test Unit”
application to facilitate this. This will prevent both people from creating long queues and to lock the
waiting rooms before being full. Also it will provide an efficient and fair sharing because units will
contain people up to its capacity.
