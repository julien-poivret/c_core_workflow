#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <time.h>

/*
    (Archives of 'my' Core workflow reference) in Pure c for development on "Embedded" linux
                            ( i mean non real time critical task )
         just about launching iot applications with the perfect runtime/energy cost.
on every kind of machine, the more efficently possible in regard of energy consumption and speed...
   the major security issue come from the lack of buffer counting and that is up to the programer 
                    to don't exceed the memory range in read/write memory, 
	       upond that bare metal at cpu core register is the safest concept !
	(perfect for beagel board, for efficent compilation time and runtime memory cost.)
		    what about Rust well Rust is a Glock and c a 357 magnum !
          ( Gun is bad ! video game are cool ! and i love 3d arts computing by the way ! )
      
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
// linux timing declaration prototype ...
// (just tell to the compiller how the lib time.h shape data)
//  *** uncoment the following line for avoiding implicit warning declaration at compile time. on Linux machine don't needed on mac OSX *** 
//  int nanosleep (const struct timespec *req, const struct timespec *rem);

// Shared memory with a life time equal to the main function.
// typedef avoid to recall the key word struct Data_thread at declaration only Data_thread is call. 
typedef struct
{
  pthread_mutex_t mut;
  pthread_mutex_t mut_b;
  pthread_cond_t wai;
  bool flag1;
  atomic_bool flag2;
  char bytes_stream[50];
  struct timespec* timer1;
} Data_thread;


void * thread_1 (void *arg)
{
  /*
    - This thread (th1) will display my name byte after byte, with a pause 200ms
     in between: from stack to shared memory initialized in the main thread. (in main function)
     the full name is also display in one shot, once the whole buffer is copied with a classic prinf.
    - At the end a singnal for starting the thread 2 is send, for that a pthread_mutex_t and a 
     pthread_cond_t are shared through a data structure of typedef Data_thread.
     the three threads main ,thread1 and thread2 can acces to the data via a pointer. 
  */
	
  // there a consecutive 20 bytes on stack are initialized, only 6 are used 14 bytes are so wasted
  // (not a big deal) for the show case.
  char Name[20] = "Julien";
	
  // There is the crutial point: a raw (void*) pointer is casted as (Data_thread*) pointer in 
  // order to read the bytes interval pointed by (void*)arg as struct Data_thread when Dereferenced...
  // ( that the way how c pass arguments between threads. )
  Data_thread* access_th1 = (Data_thread *) arg;
  
  // the folowing memory pointer point on 50 bytes non initialized 
  // so to avoid random flotting memory state the whole 50 bytes are initialized to zero.
  for(size_t i =0;i<50;i++){
   *(access_th1->bytes_stream+i) = '\0';
  }

  // Now we can acces to the shared memory (while the following still unsafe: "any other thread can potentially access to that 
  // memory area", even if in the case of this program (as it's writed), no other acces are claimed, so it's ok but unsafe
  // it's where bug grow and it's what Rust runtime avoid by borrow checking at compile time.
  access_th1->flag1 = false;
 
  // There a safe atomic instruction protect the data from concurent access via an atomic_bool type
  // it's memory safe no concurent acces can be made.
  access_th1->flag2 = true;
  
  printf ("\x1b[1;1H\33[2JCopy name to shared memory...\n");
  for (size_t i = 0; i < 6; i++)
    {
      // Safe memory access with controled length buffer as my name contain 6 bytes.
      // so the loop will copy only 6 bytes (0->5 size_t) from stack.
      // Also a mutual exclution mutex forbid other thread sharing the same mutex to acces to this memory area
      // defined between lock and unlock. 
      pthread_mutex_lock (&access_th1->mut);
      *(access_th1->bytes_stream + i) = *(Name + i);
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

  printf ("Hello from thread 1 ! %s atomic flag->%d\n", access_th1->bytes_stream,access_th1->flag2);
  // Start the thread 2.
  pthread_cond_signal (&access_th1->wai);
  return NULL;
}

void * thread_2 (void *arg)
{
  Data_thread *access_th2 = (Data_thread *) arg;
  // Wait until thread1 send the signal to start. not much energy will be consumed while waiting.
  pthread_mutex_lock (&access_th2->mut_b); // Lock this kind of specific following action.
  pthread_cond_wait (&access_th2->wai, &access_th2->mut_b); // pause the thread2 and wait signal to start
	                                                    // also unlock the specific mutex "mut_b"
	                                                    // (garant of the non thread race in 
	                                                    // the whole frame of running thread )
  //thread 2 runtime... can be any thing...
  printf ("Hello from thread 2 !\n");
  fflush (stdout);
  return NULL;
}

int main (int argc, char *argv[])
{

  char help[64] =
    "Basic Documentation.\n"
    "        Test        \n" 
    "     19-Nov-21      \n";

  // arguments handling like in c++.
  if( argc==2 && argv[1][0]=='-' && argv[1][1]=='h' ){
    // Basic Error Handling.
    if (!((size_t) write (1, (char *) help, 64)))
    {
      fprintf (stderr, "Error stdout !");
      return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
  }
  
  // linux timer structure for time sheduling across threads.
  struct timespec time_ch;
  time_ch.tv_sec = 0;
  time_ch.tv_nsec = 200000000; //200ms in ns
  
  //shared memory initialization.
  Data_thread data;
  data.timer1 = &time_ch;
  
  //two threads are declared.
  pthread_t th1;
  pthread_t th2;
  //Mutex and thread Signal comunication are initialized.
  pthread_mutex_init (&data.mut, NULL);
  pthread_mutex_init (&data.mut_b, NULL);
  pthread_cond_init (&data.wai, NULL);

  // Launch the threads;
  pthread_create (&th1, NULL, &thread_1, (void *) &data);
  pthread_create (&th2, NULL, &thread_2, (void *) &data);

  // Join them. (both must have finish before exiting main.)
  pthread_join (th1, NULL);
  pthread_join (th2, NULL);

  return EXIT_SUCCESS;
}
