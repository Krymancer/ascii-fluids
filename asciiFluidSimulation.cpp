#include <iostream>
#include <unistd.h>
#include <cmath>

#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 24

int areascanx = 0, areascany = 0;
double posx[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double posy[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double density[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
int wallflag[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double forcex[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double forcey[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double velocityx[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double velocityy[CONSOLE_WIDTH * CONSOLE_HEIGHT * 2];
double distancex, distancey, interaction, distance;

int	x, y, screenBufferIndex, particles;
int	gravity = 1, pressure = 4, viscosity = 7;

char screenBuffer[CONSOLE_WIDTH * CONSOLE_HEIGHT + 1];


int main(){

    // clear screen
    puts("\x1b[2J");

	// read input
	int particlesCounter = 0;
	while ((x = getc(stdin)) != EOF) {

		switch ( x ) {
			case '\n':
		        // next row
				// rewind the x to -1 cause it's gonna be incremented at the
				// end of the while body
		        areascany += 2;
		        areascanx = -1;
			 	break;
			case ' ':
				break;
			case '#':
				// The character # represents “wall particle” (a particle with fixed position),
				// and any other non-space characters represent free position particles.
				wallflag[particlesCounter] = wallflag[particlesCounter + 1] = 1;
			default:
        		posx[particlesCounter] = areascanx;
        		posy[particlesCounter] = areascany;

        		posx[particlesCounter + 1] = areascanx;
        		posy[particlesCounter + 1] = areascany + 1;

				// we just added two particles
				particles = particlesCounter += 2;

		}
        // next column
        areascanx += 1;
 
    }

    while (1) { 

		int particlesCursor, particlesCursor2;

        // Iterate over every pair of particles to calculate the densities
		for (particlesCursor = 0; particlesCursor < particles; particlesCursor++){
			// density of "wall" particles is high, other particles will bounce off them.
			density[particlesCursor] = wallflag[particlesCursor] * 9;

			for (particlesCursor2 = 0; particlesCursor2 < particles; particlesCursor2++){

				distancex = posx[particlesCursor] - posx[particlesCursor2];
				distancey = posy[particlesCursor] - posy[particlesCursor2];
				distance = sqrt( pow(distancex,2.0) + pow(distancey,2.0));
				interaction = distance / 2.0 - 1.0;

				// this line here with the alternative test
				// works much better visually but breaks simmetry with the
				// next block
				//if (round(creal(interaction)) < 1){
				// density is updated only if particles are close enough
				if (floor(1.0 - interaction) > 0){
					density[particlesCursor] += interaction * interaction;
                }
            }
        }

        // Iterate over every pair of particles to calculate the forces
		for (particlesCursor = 0; particlesCursor < particles; particlesCursor++){
			forcey[particlesCursor] = gravity;
			forcex[particlesCursor] = 0;

			for (particlesCursor2 = 0; particlesCursor2 < particles; particlesCursor2++){

				distancex = posx[particlesCursor] - posx[particlesCursor2];
				distancey = posy[particlesCursor] - posy[particlesCursor2];
				distance = sqrt( pow(distancex,2.0) + pow(distancey,2.0));
				interaction = distance / 2.0 - 1.0;

				// force is updated only if particles are close enough
				if (floor(1.0 - interaction) > 0){
					forcex[particlesCursor] += interaction * (distancex * (3 - density[particlesCursor] - density[particlesCursor2]) * pressure + velocityx[particlesCursor] *
					  viscosity - velocityx[particlesCursor2] * viscosity) / density[particlesCursor];
					forcey[particlesCursor] += interaction * (distancey * (3 - density[particlesCursor] - density[particlesCursor2]) * pressure + velocityy[particlesCursor] *
					  viscosity - velocityy[particlesCursor2] * viscosity) / density[particlesCursor];
                }
            }
        }


		// empty the buffer
		for (screenBufferIndex = 0; screenBufferIndex < CONSOLE_WIDTH * CONSOLE_HEIGHT; screenBufferIndex++){
			screenBuffer[screenBufferIndex] = 0;
        }


		for (particlesCursor = 0; particlesCursor < particles; particlesCursor++) {

			if (!wallflag[particlesCursor]) {
				
				// Newtonian mechanics 

				// force affects velocity
				if ( sqrt( pow(forcex[particlesCursor] ,2.0) + pow(forcey[particlesCursor],2.0)) < 4.2) {
					velocityx[particlesCursor] += forcex[particlesCursor] / 10;
					velocityy[particlesCursor] += forcey[particlesCursor] / 10;
				}
				else {
					velocityx[particlesCursor] += forcex[particlesCursor] / 11;
					velocityy[particlesCursor] += forcey[particlesCursor] / 11;
				}

				// velocity affects position
				posx[particlesCursor] += velocityx[particlesCursor];
				posy[particlesCursor] += velocityy[particlesCursor];
			}

			x = posx[particlesCursor];

			y = posy[particlesCursor]/2;
			screenBufferIndex = x + CONSOLE_WIDTH * y;

			if (y >= 0 && y < CONSOLE_HEIGHT - 1 && x >= 0 && x < CONSOLE_WIDTH - 1) {
				screenBuffer[screenBufferIndex]   |= 8; // set 4th bit to 1
				screenBuffer[screenBufferIndex+1] |= 4; // set 3rd bit to 1
				// now the cell in row below
				screenBuffer[screenBufferIndex + CONSOLE_WIDTH]   |= 2; // set 2nd bit to 1
				screenBuffer[screenBufferIndex + CONSOLE_WIDTH + 1] |= 1; // set 1st bit to 1
			}
			
        }

		// Update the frame
		for (screenBufferIndex = 0; screenBufferIndex < CONSOLE_WIDTH * CONSOLE_HEIGHT; screenBufferIndex++){
			if ( screenBufferIndex % CONSOLE_WIDTH == CONSOLE_WIDTH - 1)
				screenBuffer[screenBufferIndex] = '\n';
			else
				// the string below contains 16 characters, which is for all
				// the possible combinations of values in the screenbuffer
				screenBuffer[screenBufferIndex] = " '`-.|//,\\|\\_\\/#"[screenBuffer[screenBufferIndex]];
				// ----------- mapping -----------
				// 0  => space
				// 1  => '    2  => `    3  => -
				// 4  => .    5  => |    6  => /
				// 7  => /    8  => ,    9  => \
				// 10 => |    11 => \    12 => _
				// 13 => \    14 => /    15 => #
        }

        // back to the top left of the screen
        puts("\x1b[1;1H");
        // draw the frame
        puts(screenBuffer);

        // delay
		usleep(3000);
	} 
    
}