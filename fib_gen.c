/**
 * @file   fib_gen.c
 * @Author Denis Pynkin (d4s),  denis.pynkin@t-linux.by
 * @brief  
 * @copyright Copyright (c) 2015, Denis Pynkin
 * @license This project is released under the GNU Public License.
 *
*/


#include <stdio.h>
#include <stdlib.h>

int main(void) {

	long long freq=1;

	for( int i='A'; i<'A'+30; i++) {
//		printf("%c", i);

		fprintf(stderr, "i=%c,  freq=%ld\n", i, freq);
		for( int j=0;j<freq;j++) printf("%c",i);
		freq*=2;
	}
	return 0;
}

