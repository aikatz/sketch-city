# Create a Sine Table in One* Line of Python

*The generation happens in one line, but it also needs a couple of imports.

NOTE: This tutorial uses Python 2.7.x

As part of the DDS on the FPGA for Lab 3, each group needs to make a ROM in Verilog to hold discrete values of the sine function, i.e. a sine table. The general idea is to calculate N discrete values of the sine function over one period. The general formula for N discrete sine values over one period would be `sin(n*2*pi/N)` where `n=1,2,3...N`. Since we want our output to be between 0 and 255, in order for it to fit into one byte, we need to multiply the sine function by 127, and then shift it upwards by 127. This gives us `127*sin(n*2*pi/N)+127`. Since we will need to truncate the value to an integer, this will really be between 0 and 254, unless the value passed to sin() _exactly_ 1/4 or 3/4 the period.

Most of the sine table module is trivial to write in Verilog, so the only part we will focus on generating is setting the values in the ROM. Let's assume we are using the following Verilog code from Team Alpha's website:

```verilog
module SINE_ROM
(
  input [9:0] addr,
  input clk, 
  output reg [7:0] q
);

  // Declare the ROM variable
  reg [7:0] sine[628:0];

  initial
  begin
     sine[0] <= 8'b10000000;
     //... remaining contents of sine table//
     sine[628] <= 8'b10000000;
  end

  // Read from requested address of ROM
  always @ (posedge clk)
  begin
    q <= sine[addr];
  end
endmodule
```

We are going to generate the part inside the initial block, to make it easy we will simply print the output to stdout and assume the user can copy/paste correctly. Python has a really useful string formatting feature, which can be used to insert variables into a string. For example the following snippet prints `"My name is Alex and the current year is 2017"`:

```
str = "My name is {} and the current year is {}".format("Alex", 2017)
print(str)
```

Applying this to our sine function and Verilog syntax, we get the expression: `127*math.sin(n*2*math.pi/N)+127`. Due to the way that Python handles types the output of this function is actually between 0 and 256, if we truncate the first part of the expression by converting it to an int, we have `int(127*math.sin(n*2*math.pi/N))+127`, and now the output is between 0 and 255. Now we need to to generate the values of n, so we can just use a loop, right? Well, if you use a loop the code looks like this:

```
for n in xrange(N):
  print("sine[{}] <= 8'd{};".format(n, int(127*math.sin(n*math.pi/N)+127)))
```

But that's two lines, not one! Yes, you could just move the second line up to the first and it would still work, but that's boring and everyone already knows how to write a for loop. Instead you can use Python's [list comprehensions](https://www.python.org/dev/peps/pep-0202/) feature! List comprehensions are a fancy way to create lists in Python, and they can be used to construct lists using multiple for-loops and if-statements in a single line. Our use case is rather simple, and using list comprehension doesn't necessarily improve the code, however they can be very powerful when used correctly, and this serves as a simple introduction. With that being said, here is what the code looks like using list comprehension:

```
[print("sine[{}] <= 8'd{};".format(n, int(127*math.sin(n*math.pi/N)+127))) for n in xrange(N)]
```

One _very_ important note is that since `print` is a statement, not a function, in Python 2.7.x, you need to add the following line of code to the top of your file:

```
from __future__ import print_function
```

Which will import the version of `print` that is a function, not a statement, which will allow you to use the list comprehension syntax above. If you are curious about using list-comprehension in more complex scenarios, take a look at the link above! Here is the full code including the imports, assuming `N=256`.

```
from __future__ import print_function
import math

[print("sine[{}] <= 8'd{};".format(n, int(127*math.sin(n*math.pi/256)+127))) for n in xrange(256)]
```
