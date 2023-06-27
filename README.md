# Calculator 2.0
A simple syntax-directed calculator

NEW PATCH 3.0

Bugs Fix:
1) 2^3 give 2x^3 instead of 8.00
2) Simplify nodes in nodes does not valid
	- The 2nd version simplify function does not verify the difference in inner nodes.
	- Verifying its inner term's identical or not is inefficient and error prone.
	- Implement recursive function based on 2nd version's simplify method.
3) Remove remnants nodes like sfactor and trigfactor


New Features:
1) Enable basic trigonometric functions calculations. Ex: sin(cos(x))*cos(2*x)
2) Polynomial division now available. Ex: x/2 (simple division only, not x/(2*x + 1)
3) Double equations realization. Ex: x^2 = 0 ; 2*x = -1 addition ==> x^2 - 2*x + 1 (output)
4) Avoid syntax error and program can only terminate via Ctrl+C interrupt
5) New function "Compute": Ex: x^2 + sin(pi) compute 3 ==> 9 (output)
6) Special numeric characters. Ex: pi == 3.14159


# Basic Functions & EXample
1) print
   - input : cos(x)*sin(x) print
   - output: 0.50sin(x^1.00 + x^1.00) -0.50sin(x^1.00 -1.00x^1.00)
2) addition 
   - input : sin(x) = 2 ; cos(sin(x)) = 3 addition
   - output: 1.00sin(x^1.00) -5.00 + 1.00cos(1.00sin(x^1.00))
3) simplify: 
   - input : sin(sin(sin(x-x))) simplify
   - output: 0.00
4) integrate (limited to linear polynomial only)
   - input : 3*x^2 + x = 0 integrate
   - output: x^3.00 + 0.50x^2.00 + C^1.00
5) derivative
   - input : 2*sin(x^3 + 2*x + 1) derivative
   - output: 6.00x^2.00cos(x^3.00 + 2.00x^1.00 + 1.00) + 4.00cos(x^3.00 + 2.00x^1.00 + 1.00)

6) solve
   - input : (x-22)^5 solve
   - output: The approximate solution after 100 iterations: x = 21.997941
7) compute
   - input : 0.5*sin(pi) + x^10 compute 2
   - output: Answer: 1024.00000
