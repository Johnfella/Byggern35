#ifndef __MACROS_H
#define __MACROS_H

#define set_bit( reg , bit ) ( reg |= ( 1 << bit ) )
#define clear_bit( reg , bit ) ( reg &= ~( 1 << bit ) )
#define test_bit( reg , bit ) ( reg & ( 1 << bit ) )

#endif