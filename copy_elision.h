#ifndef COPY_ELISION_H
#define COPY_ELISION_H


/*
 *
 * Under some circumstances, the compilers are permitted, but not required to omit
 * the copy- and move- (since C++11)construction of class objects even if
 * the copy/move (since C++11) constructor and the destructor have observable side-effects.
 * This is an optimization: even when it takes place and the copy-/move-constructor is not called,
 * it still must be present and accessible (as if no optimization happened at all),
 *  otherwise the program is ill-formed.
 *
*/

void testCopyElision();

#endif // COPY_ELISION_H
