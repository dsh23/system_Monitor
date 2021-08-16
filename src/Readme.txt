General notes on design decisions and code organisation.

1. Given that this project was delivered as a finale to the sections
that introduced OO principles and practices I have decide to use
private class instance variables the processor and process classes and
provide access to these via discrete getter/setter mechanisms

2. Build a stage 1 branch to incorporate only the work from the
processor class. Then build a stage 2 branch to incorporate the work
on the process class. Merge these into a staging submission.

3. Investigate additional options and merge these where possible

4. I have stored variables where it feels like wisdom to prevent
continuous poling for constant information. Notably, if I were to
extend this project it might be prudent to store more attributes of
the Process class in order to facilitate extended operations. For now
as these attributes are generally dynamic I will pass them thraight
through to the renderer. 

5. Much of my ideas changed over time. Particularly as I tried to be accurate with
the results and began to debug the results of various implementations
that were taking too much time.
