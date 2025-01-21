WATER JUG PUZZLE:

Three water jugs A, B, C are given with their maximum capacity. Only jug C starts completely filled with water; the others are empty.
Given a target goal where each jug has exactly the specified amount of water, and heeding rules about how water may be transferred 
(such as only pouring until a jug is full), return the shortest possible step-by-step solution (if one exists) 
of how to pour the water to reach this group state.

EX: 
START STATE: Jug A has a capacity of 3; jug B has a capacity of 5; jug C has a capacity of 9. 
END STATE: A=0, B=2, C=7

SOLUTION:
1. Pour 5 gallons from jug C into jug B. A=0, B=5, C=4
2. Pour 3 gallons from jug B into jug A. A=3, B=2, C=4
3. Pour 3 gallons from jug A into jug C. A=0, B=2, C=7
