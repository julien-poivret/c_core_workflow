#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<pthread.h>
#include<stdatomic.h>
#include <stdbool.h>
#include <time.h>

/*
     Archives of 'my' Core workflow for reference in Pure c for development on "Embedded" linux
                            ( i mean non real time critical task )
         just about launching iot applications with the perfect runtime/energy cost.
on every kind of machine, the more efficently possible in regard of energy consumption and speed...
   the major security issue come from the lack of buffer counting and that is up to the programer 
                    to don't exceed the memory range in read/write memory, 
	       upond that bare metal at cpu core register is the safest concept !
	(perfect for beagel board, for efficent compilation time and runtime memory cost.)
		    what about Rust whell Rust is a Glock and c a 357 magnum !
          ( Gun is bad ! video game are cool ! and i love 3d arts coputing by the way ! )
      
                                            .____.
                                         xuu$``$$$uuu.
                                       . $``$  $$$`$$$
                                      dP*$  $  $$$ $$$
                                      ?k $  $  $$$ $$$
                                       $ $  $  $$$ $$$
                                       ":$  $  $$$ $$$
                                        N$  $  $$$ $$$
                                        $$  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $$#$  $$$ $$$
                                         $$'$  $$$ $$$
                                         $$`R  $$$ $$$
                                         $$$&  $$$ $$$
                                         $#*$  $$$ $$$
                                         $  $  $$$ @$$
                                         $  $  $$$ $$$
                                         $  $  $$$ $$$
                                         $  $  $B$ $$&.
                                         $  $  $D$ $$$$$muL.
                                         $  $  $Q$ $$$$$  `"**mu..
                                         $  $  $R$ $$$$$    k  `$$*t
                                         $  @  $$$ $$$$$    k   $$!4
                                         $ x$uu@B8u$NB@$uuuu6...$$X?
                                         $ $(`RF`$`````R$ $$5`"""#"R
                                         $ $" M$ $     $$ $$$      ?
                                         $ $  ?$ $     T$ $$$      $
                                         $ $F H$ $     M$ $$K      $  ..
                                         $ $L $$ $     $$ $$R.     "d$$$$Ns.
                                         $ $~ $$ $     N$ $$X      ."    "%2h
                                         $ 4k f  $     *$ $$&      R       "iN
                                         $ $$ %uz!     tuuR$$:     Buu      ?`:
                                         $ $F          $??$8B      | '*Ned*$~L$
                                         $ $k          $'@$$$      |$.suu+!' !$
                                         $ ?N          $'$$@$      $*`      d:"
                                         $ dL..........M.$&$$      5       d"P
                                       ..$.^"*I$RR*$C""??77*?      "nu...n*L*
                                      '$C"R   ``""!$*@#""` .uor    bu8BUU+!`
                                      '*@m@.       *d"     *$Rouxxd"```$
                                           R*@mu.           "#$R *$    !
                                           *%x. "*L               $     %.
                                              "N  `%.      ...u.d!` ..ue$$$o..
                                               @    ".    $*"""" .u$$$$$$$$$$$$beu...
                                              8  .mL %  :R`     x$$$$$$$$$$$$$$$$$$$$$$$$$$WmeemeeWc
                                             |$e!" "s:k 4      d$N"`"#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                             $$      "N @      $?$    F$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                             $@       ^%Uu..   R#8buu$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                        ```""*u$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                               #$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                                "5$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                                  `*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                                    ^#$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                                       "*$$$$$$$$$$$$$$$$$$$$$$$$$$>
                                                                         `"*$$$$$$$$$$$$$$$$$$$$$$$>
                                                                             ^!$$$$$$$$$$$$$$$$$$$$>
                                                                                 `"#+$$$$$$$$$$$$$$>
                                                                                       ""**$$$$$$$$>
                                                                                              ```""
*/

int nanosleep (const struct timespec *req, const struct timespec *rem);

typedef struct
{
  pthread_mutex_t mut;
  pthread_cond_t wai;
  bool flag1;
  char bytes_stram1[50];
  struct timespec *timer1;
} Data_thread;


void * thread_1 (void *arg)
{

  // Thread 1.
  char Name[20] = "Julien";
  Data_thread *access_th1 = (Data_thread *) arg;
  access_th1->flag1 = false;
  access_th1->timer1->tv_nsec = 200000000;
  access_th1->timer1->tv_sec = 0;

  printf ("\x1b[1;1H\33[2JCompute name...\n");
  for (size_t i = 0; i < 6; i++)
    {
      pthread_mutex_lock (&access_th1->mut);
      *(access_th1->bytes_stram1 + i) = *(Name + i);
      pthread_mutex_unlock (&access_th1->mut);
      if (i == 5)
	{
	  printf ("%c\n", *(Name + i));
	}
      else
	{
	  printf ("%c,", *(Name + i));
	}
      fflush (stdout);
      nanosleep (access_th1->timer1, access_th1->timer1);
    }

  printf ("Hello from thread 1 ! %s\n", access_th1->bytes_stram1);
  pthread_cond_signal (&access_th1->wai);
  return NULL;
}

void * thread_2 (void *arg)
{
  Data_thread *access_th2 = (Data_thread *) arg;
  pthread_cond_wait (&access_th2->wai, &access_th2->mut);
  // Thread 2.
  printf ("Hello from thread 2 !\n");
  fflush (stdout);
  return NULL;
}

int main (int argc, char *argv[])
{

  char help[64] =
    "Basic Documentation.\n"
    "        Test        \n" "     19-Nov-21      \n";

  // Basic Error Handling.
  if (!((size_t) write (1, (char *) help, 64)))
    {
      fprintf (stderr, "Error stdout !");
      return EXIT_FAILURE;
    };

  // Set the Data Stream for thread. 
  struct timespec time_ch;
  time_ch.tv_sec = 1;
  time_ch.tv_nsec = 1;
  Data_thread data;
  data.timer1 = &time_ch;
  pthread_t th1;
  pthread_t th2;
  pthread_mutex_init (&data.mut, NULL);
  pthread_cond_init (&data.wai, NULL);

  // Launch stream;
  pthread_create (&th1, NULL, &thread_1, (void *) &data);
  pthread_create (&th2, NULL, &thread_2, (void *) &data);

  // Join them.
  pthread_join (th1, NULL);
  pthread_join (th2, NULL);

  return EXIT_SUCCESS;
}
