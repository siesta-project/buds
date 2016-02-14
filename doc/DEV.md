
# General development

Anybody is encouraged to contribute to refype in any way they feel.

We welcome:

- Finding issues
- Reporting bugs
- Advice on extensions


## Notes for developers

We target the code to be as clear as possible and as self-interpreted as well.
In general the guide-lines for the code __must__ be followed:

- Indent in a 2-level sequence.
  - This is enabled by variable tracking in the files
  - For vi/emacs users this is already enabled
- Pre-processor variables __must__ adhere to these rules:
  1. Prefix with \_R\_
- Document every routine via the regular Doxygen statements.
  If you do not know how Doxygen works, please look in the documentation
  for a routine/function which you think replicates your routine the most
  and copy/paste the corresponding documentation.

## Creation a new refype

There are numerous ways to attack this problem.

In the following a description of the mryp_Array types are followed.
My recommendations is to follow that implementation to adhere to
some sort of standard for the data structures.

1. The first code present in the type _must_ be

```
   #include "refype_common_declarations.inc"
```

2. Define data structure.
   Define two types, one you _actual_ data structure name (tryp\_<Array1D>),
   and the other with an underscore appended (which is the actual
   data container).
   
   The data-type should _always_ look like this:
   ```fortran
   type <type>
      type(<type>_), pointer :: data => null()
   end type
   ```
   And the <type>\_ should _always_ have this header:
   ```fortran
   type <type>_
   #   include "refype_type_common.inc"
      <other contained data>
   end type
   ```

3. Secondly the specific declarations for the module goes right after.
   You may perform any kind of preprocessing of variables at this point.

4. Input the common refype codes.
   ```fortran
    #include "refype_common.inc"
   ```
   This separates the declarations from the routines. I.e. `contains`
   is present in the included file.

5. Add `delete_data` routine to enable a clean-up of the data.

6. Add all routines specific for this data-type.


