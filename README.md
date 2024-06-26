<h1>Welcome to "python--"!</h1>
<h5>! This is in no shape or form associated with the real python project and not meant to ridicule it, I was just lazy with the name. !</h5>
<h3>What is it?</h3>
This heresy of a language has been (or is being) created by me as an alternative form of passing my C++ exercises at Rzeszów University of Technology, also for passing Advanced C++ laboratories. It's loosely based on python's syntax with some personal touches.
<h3>And what does it do?</h3>
It's a simple interpreted language, with support for reading .p-- files and reading user input directly. It's supposed to be able to do simple math and print operations, maybye file saving/reading.
<h3>Why is everything in english if this is for a polish university?</h3>
Why not? I like using english in programming.
<h2>Features:</h2>
<ul>
<h5>Note: these features are subject to change at any moment</h5>
<li>functions can be defined by user</li>
<li>there are global and local variables</li>
<li>function recursion is supported</li>
<li>functions can implicitly convert all inputs to a given type if possible</li>
<li>there is a standard library calling native C++ functions</li>
<li>interpreter uses standard functions or has variants for at least Linux and Windows</li>
<li>can be called from bash/console or directly</li>
<li>function arguments not written in a bracket can be treated as multiple variables in some cases</li>
</ul>
<h2>Syntax:</h2>
A "Hello world" in python--:

```
Print("Hello world!")
> Hello world!
```
Nothing special, right? </br>
Then feast your eyes upon this unholy print statement:

```
Print( 5 + " + " + 6 + " = " + (5 + 6))
> 5 + 6 = 11
```
What? </br>
In python-- Print() has the "implicit" keyword before it's string argument, it converts all its arguments to strings implicitly. 

```
Print(implicit string message)
```
All the bracket-less arguments are affected and then added. This empowers the user to create incredible feats of lexical gymnastics like this in a single statement </br>
You can also see the next difference: functions are typed and don't need ':' after them. Actually this makes writing multiple expressions in a line impossible, and the parser much simpler. </br>
Some functions have c++ native binds, these are realised via a special type of function, not possible to create from the language itself and are fully separate from user functions.</br></br>
This function doesn't return any value, but when one does, it does so explicitly:
```
num StringLength(implicit string str)
```
Yeah, there are no objects.</br></br>
Indentations are important:
```
implicit num foo(num a, string b)
    return a + b
Println(foo(3, "4"))
Print("This is so silly!")
> 7
> This is so silly!
```
The keyword "implicit" can be used anywhere with variables, but in some cases it can generate exceptions:
```
implicit num foo(string a)
    a = a + "not a number"
    return a <- exception here
    
implicit num b = 5 + " + " + 7 + " = " + (5 + 7) <- exception here
```
The "implicit" keyword generally allows for type conversions:
```
num var = 5
string str = "This equals "
implicit str += var;
```

### Now, did you think that's all? It isn't!

After the latest batch of work Python-- now supports switch statement, arrays, file reading via functions and system console calls!

Arrays are declared like this:

```
num array[] = [1, 2, 3, 4, 5]
```

Arrays are indexed from 0 and that's about the only normal thing about the behaviour of those godforsaken constructs.

They support negative indexing. I haven't explicitly added support for two-dimensional arrays, but I guess it could be possible with enough determination to force this abomination to allow for it. Now, wanna print an array?

```
num arr[] = [1, 2, 3]
num n = 0;
while (num < arr:size)
    Println((arr[n]))
    n += 1
end
```
Wait, but shouldn't it be Println(arr[n])?

NOPE, with the way implicit works it quite a bit of work to make it function in other ways, or at least I don't want to put this work in,

## So how to make this glorious thing reside on my device?
In the latest release there's a x86_64 executable for linux (compiled on fedora to be precise). I won't bother myself with compiling it on windows unless asked, there's nothing stopping you from compiling it for anything though! There are no platform specific libraries or commands used!

Once you link it to your bash you can use it as a standalone command or as an interpreter program!