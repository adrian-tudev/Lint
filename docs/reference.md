# Lint Reference

This reference covers the core semantics and syntax of the "interpreted" programing language *Lint*.

## Literals

- **Strings** should be enclosed between *two* quotation marks.
- **Numerical** values should contain decimal digits using period to indicate floating point values.
- **Booleans** are denoted by *true* and *false*.

> Example:
>
> ```hs
> -- String
> "This is a string!" 
>
> -- Integer
> 67  
>
> -- Float
> 13.37
>
> -- Boolean
> true   
> ```

## Variables

Variables are defined using the **let** keyword (shoutout Rust) \

> Example:
>
> ```
> let x = 10;
> ```

## Operators (Arithmetic and Boolean)

The usual operators for simple arithmetic `+-/*`. Booleans are operated upon by `and`, `or` and `not`. Comparisons are made through `==` and evaluates to a boolean value.Compare inequality by using `!=` and `!` to invert a boolean value (equivalent to the `not` keyword ). Lint offers numerical comparisons through the standard notation `<`, `>` and all combinations of the equal sign (`<=`, `>=`).

> Example:
>
> ```
>> 1 + 1
> 2
>
>> true and false
> false
>
> ```

## Functions

> Example:
>
> ```
> fn add(a b) {
>     ret a + b;
> }
> ```

## Control Flow

The flow of a program is controlled by `if`, `elif` and `else`. Note that the expression always need to evaluate to a boolean value

> Example:
>
> ```
> if (*expression*) {
>   *do something*
> }
> ```

## Loops

There are two kind of loops: `for` and `while`.
For convenience sake, the keywords `skip` and `break` allow for control flow in the loop body.

## Comments

```
-- Single line comment

-!
Multiline
comment
!-
```

## Keywords

Following are all the reserved keywords.

- `and`
- `or`
- `not`

## Standard Library

Not implemented yet.
