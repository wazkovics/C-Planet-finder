#include "space_explorer.h"
#include <stdlib.h>

#include <stdio.h>



struct lowest_distance_from_mixer{
    double shortest_mixer_distance;
    double current_main_mixer_distance;
    double *visitedplanetsmixerdistances;
    double mixerdistance;
    unsigned int current_main_crt;
    unsigned int shortest_planet_coordinate;
    unsigned int *connections_to_others;
    unsigned int *current_shortest_connections;
    unsigned int *visitedplanets;
    unsigned int *deadends;
    int numberofdeadends;
    int number_of_visited_planets;
    int current_shortest_cons;
    int number_connections;
    int connection_index;
    int randomhopcounter;
    int newplanet;
    int backtracking;
};


ShipAction space_hop(unsigned int crt_planet, // planet ID
                   unsigned int *connections, // paths to other planets
                   int num_connections, // number of paths to other planets
                   double distance_from_mixer, // distance from mixer
                   void *ship_state)
{
    /* your solution here */
    if(ship_state == NULL){
        //struct lowest_distance_from_mixer ldfm = {distance_from_mixer,crt_planet,1};
        struct lowest_distance_from_mixer *ldfm = malloc(sizeof(struct lowest_distance_from_mixer));
        ldfm->connection_index = 0;
        ldfm->shortest_mixer_distance = distance_from_mixer;
        ldfm->shortest_planet_coordinate = crt_planet;
        ldfm->connections_to_others = malloc(num_connections * sizeof(unsigned int));
        for(int i=0;i<num_connections;i++){
            ldfm->connections_to_others[i] = connections[i];
        }
        ldfm->number_connections = num_connections;
        ldfm->randomhopcounter = 1;
        ldfm->newplanet = 0;
        ldfm->numberofdeadends = 0;
        ship_state = ldfm;
//       printf("%d Ship state null \n",crt_planet);
        struct ship_action next_action = {RAND_PLANET,ship_state};
        return next_action;
    }
    struct lowest_distance_from_mixer *ldfm = (struct lowest_distance_from_mixer *)ship_state;



    //doing some random hops to get closer to the mixer if possible
    if(ldfm->shortest_mixer_distance >2){
        if(ldfm->shortest_mixer_distance>distance_from_mixer){
            //printf("Previous distance from mixer was %f units\n Now its %f\n",ldfm->shortest_mixer_distance,distance_from_mixer);
            ldfm->shortest_mixer_distance = distance_from_mixer;
            ldfm->shortest_planet_coordinate = crt_planet;
            free(ldfm->connections_to_others);
            ldfm->connections_to_others = malloc(num_connections * sizeof(unsigned int));
            for(int i=0;i<num_connections;i++){
                ldfm->connections_to_others[i] = connections[i];
            }
            ldfm->number_connections = num_connections;
        }
        ldfm->randomhopcounter += 1;
        ldfm->newplanet = 0;
        ldfm->connection_index = 0;
        ldfm->current_main_mixer_distance = distance_from_mixer;
   //     printf("Coordinate %d , Random hop no %d \n",crt_planet,ldfm->randomhopcounter);
        struct ship_action nextplanet = {RAND_PLANET,ldfm};
        return nextplanet;
    }
    ldfm->mixerdistance = distance_from_mixer;

    //for backtracking visit the destined planet and its connections to search and go from there again
    if(ldfm->backtracking == 1){
    //    printf("CALLED BACKTRACKING\n\n\n");
        ldfm->backtracking = 0;
        ldfm->connection_index = 0;
        ldfm->mixerdistance = distance_from_mixer;
        ldfm->shortest_mixer_distance = distance_from_mixer;
        ldfm->shortest_planet_coordinate = crt_planet;
        ldfm->connections_to_others = malloc(num_connections * sizeof(unsigned int));
        for(int i=0;i<num_connections;i++){
            ldfm->connections_to_others[i] = connections[i];
        }
        ldfm->number_connections = num_connections;
        ldfm->newplanet = 0;
        ship_state = ldfm;
        struct ship_action next_action = {ldfm->connections_to_others[ldfm->connection_index],ship_state};
        return next_action;
    }

    // finding the planet with the shortest connections from main, then
    if(ldfm->shortest_mixer_distance>distance_from_mixer && num_connections >1){
    //    printf("Previous distance from mixer was %f units\n Now its %f\n",ldfm->shortest_mixer_distance,distance_from_mixer);
        ldfm->shortest_planet_coordinate = crt_planet;
        free(ldfm->current_shortest_connections);
        ldfm->current_shortest_connections = malloc(num_connections * sizeof(unsigned int));
        for(int i=0;i<num_connections;i++){
            ldfm->current_shortest_connections[i] = connections[i];
        }
        ldfm->current_shortest_cons = num_connections;
        ldfm->newplanet = 1;
        ldfm->shortest_mixer_distance = distance_from_mixer;

    }


    ldfm->connection_index +=1;

    //going to the next planet of the next one has been found
    if(ldfm->connection_index == ldfm->number_connections && ldfm->newplanet == 1){
     //   printf("NEW PLANET FOUND\n\n\n");
        ldfm->connection_index = 0;
        ldfm->newplanet = 0;
        free(ldfm->connections_to_others);
        ldfm->connections_to_others = malloc(ldfm->current_shortest_cons*sizeof(unsigned int));
        for(int i = 0;i<ldfm->current_shortest_cons;i++){
            ldfm->connections_to_others[i] = ldfm->current_shortest_connections[i];
        }
        ldfm->number_connections = ldfm->current_shortest_cons;
        ldfm->number_of_visited_planets +=1;

        //holding the planets in case of backtracking
        if(ldfm->number_of_visited_planets ==-1){

            ldfm->visitedplanets = malloc(sizeof(unsigned int*));
            ldfm->visitedplanets = &ldfm->current_main_crt;
            ldfm->visitedplanetsmixerdistances = malloc(sizeof(double *));
            ldfm->visitedplanetsmixerdistances = &ldfm->current_main_mixer_distance;

        } else{

            unsigned int *tempplanetholder = malloc((ldfm->number_of_visited_planets)* sizeof(unsigned int));
            double *tempdistanceholder = malloc(ldfm->number_of_visited_planets* sizeof(double *));

            for(int index = 0;index<ldfm->number_of_visited_planets;index++){
                if(index != (ldfm->number_of_visited_planets-1)){
                    tempplanetholder[index] = ldfm->visitedplanets[index];
                    tempdistanceholder[index] = ldfm->visitedplanetsmixerdistances[index];
                } else{
                    tempplanetholder[index] = ldfm->current_main_crt;
                    tempdistanceholder[index] = ldfm->current_main_mixer_distance;
                }

            }
            free(ldfm->visitedplanets);
            ldfm->visitedplanets = tempplanetholder;
            free(ldfm->visitedplanetsmixerdistances);
            ldfm->visitedplanetsmixerdistances = tempdistanceholder;
        }

        ldfm->current_main_crt = ldfm->shortest_planet_coordinate;
        //set the new mixer distance
        ldfm->current_main_mixer_distance = ldfm->shortest_mixer_distance;

    }
    //backtracking
    if(ldfm->connection_index == ldfm->number_connections && ldfm->newplanet == 0){
    //    printf("NO NEW PLANET\n\n\n");
        ldfm->numberofdeadends +=1;
        if(ldfm->numberofdeadends == 1){
            ldfm->deadends = malloc(sizeof(unsigned int*));
            *(ldfm->deadends) = ldfm->current_main_crt;

        }else{

            unsigned int *tempdeadplanetholder = malloc(ldfm->numberofdeadends* sizeof(unsigned int*));
            for(int index = 0;index<ldfm->numberofdeadends;index++){
                if(index != (ldfm->numberofdeadends-1)){
                    tempdeadplanetholder[index] = ldfm->deadends[index];

                } else{
                    tempdeadplanetholder[index] = ldfm->current_main_crt;
                }
            }
            //the heap corruption occurs here
            free(ldfm->deadends);
            ldfm->deadends = tempdeadplanetholder;
        }
        unsigned int nextplanetcord;
        double mixerdistance = 9999999999999;
        double isdeadend;
        //go through the previously visited planets which aren't a dead end, find the one with the lowest distance, then go to it and start again from there
        for(int visited_index = 0; visited_index<ldfm->number_of_visited_planets;visited_index++){
            isdeadend = 0;
            for(int deadendindex = 0;deadendindex<ldfm->numberofdeadends;deadendindex++){
                if( ldfm->visitedplanets[visited_index] == ldfm->deadends[deadendindex]){
                    isdeadend = 1;
                }
                if(isdeadend == 0){
                    if(ldfm->visitedplanetsmixerdistances[visited_index]<mixerdistance){
                        mixerdistance = ldfm->visitedplanetsmixerdistances[visited_index];
                        nextplanetcord = ldfm->visitedplanets[visited_index];
                    }
                }
            }

        }
        ldfm->backtracking = 1;

        //struct ship_action nextplanet = {nextplanetcord,ship_state};
        struct ship_action nextplanet = {RAND_PLANET,ship_state};
        return nextplanet;


    }

   // printf("Distance from mixer %f units\n",ldfm->shortest_mixer_distance);
    //printf("Number of connections %d\n",num_connections);
    //checking if next connection is dead
    int isnextdead = 1;
    while (isnextdead == 1 && ldfm->numberofdeadends>1){
        for(int deadendindex = 0;deadendindex<ldfm->numberofdeadends;deadendindex++){

            if( ldfm->connections_to_others[ldfm->connection_index] == ldfm->deadends[deadendindex]){
                ldfm->connection_index +=1;
                if(ldfm->connection_index== ldfm->number_connections){
                    ldfm->backtracking = 1;
                    struct ship_action nextplanet = {RAND_PLANET,ship_state};// ship action holds next planet coordinates and shipstate
                    return nextplanet;
                }
            } else{
                isnextdead = 0;
            }
        }
    }

    struct ship_action nextplanet = {ldfm->connections_to_others[ldfm->connection_index],ship_state};// ship action holds next planet coordinates and shipstate
    return nextplanet;
}

