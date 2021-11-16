/**! -----------------------------------------
  33 * The MIT License (MIT)

   Copyright (c) 2021 FTTech

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

   -----------------------------------------

   Test commands to tile

   This example enable a connection directly to the tile using the Serial Monitor.
   Refer to the documentation of the Swarm Tile to know wich commands are implemented:
   https://swarm.space/wp-content/uploads/2021/11/Swarm-Tile-Product-Manual.pdf 
   
   - Note that you do not need to write $ or *xx in any command, the library is going
     to take care of that for you.

   Itens needed:
    - FTTech SmartNode Swarm

   -----------------------------------------
*/

#include <FTTech_Swarm.h>

#define DEBUG_MODE true /* Set debug mode 0 (or false) for no feedback from the tile and 1 (or true) for feedbacks through USB serial. 
                           BE AWARE: SET TO 1 AND THE CODE WILL ONLY RUN IF THE SERIAL MONITOR IS ON */

FT_SWARM SmartNode(DEBUG_MODE); /* Debug mode needs to be activated to use BeginTest function */

void setup() {
  /* Nothing to do here, keep going */
}

void loop() {

  SmartNode.BeginTest(); /*Use the Serial Monitor at 9600 do send and read commands to the tile*/

}
