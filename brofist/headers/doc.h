/** @file doc.h Documentation header file */

/** @mainpage BROFist - BROFist: Robot Operational Fists
 *
 * Project BROFist is aimed at the creation of a toolkit for the
 * communication between SciCosLab and any kind of NXT that uses nxtOSEK
 * (Which we will call @anchor SPAM SPAM now on for shortness).
 *
 * This is achieved by the use of three linked parts:
 *
 *  - A set of SciCos blocks and linked C functions
 *  - A pc server that marshals the communications between Scicos and
 *      SPAM
 *  - A client to upload on SPAM
 *
 * A test client has been also provided to test BROFist functionality
 * without having to use SciCosLab.
 *
 * \section Compile Compilation
 *
 *      Each of the three parts has its own compilation and Usage method.
 *      
 *      \subsection BROServer BROFist Server
 *          For the BROFist Server it is as easy as doing:
\verbatim
cd srcIt also defines the values of the various operations implemented in BROFist and the encoding for the 7 NXT ports.
Implemented Operations:
make all
\endverbatim
 *      \subsection SPAMClient SPAM nxtOSEK Client
 *
 *      When compiling the SPAM Client the right path for the nxtOSEK
 *      @c ecrobot.mak file (Usually under [nxtOSEK path]/ecrobot/ecrobot.mak])
 *      should be written in line 11 inside spam/Makefile . This is not a
 *      good way of doing things, but this is how nxtOSEK team works for their
 *      projects.
 *
 *      \subsection ScicosBlocks Scicos Blocks and Functions
 *
 *      When using Scicoslab, before launching the Scicos screen the comand
\verbatim
exec BRO_Loader.sce
\endverbatim
 *      have to be ran to compile and link all the necessary files. Then,
 *      inside Scicos, to add every needed block, the Edit->Add New Block
 *      should be used with the right name in order to add the required
 *      blocks.
 *      An example file @c Comm_Test.cos has been provided to give the user an
 *      example of how the Scicos Blocks should be used (If you find some
 *      other "creative" ways to use them you should send me an eMail to
 *      make my day a better one).
 *
 *  \section Dependency
 *      
 *      BROFist Project needs, as you might have suggested, SciCosLab and
 *      nxtOSEK. Is it also possible that you don't have the Bluez library.
 *      Install it will be as easy as typing
\verbatim
sudo apt-get install libbluetooth-dev
\endverbatim
*       However it shouldn't be necessary as it should be installed
*       automatically in your system.
*
*   \section WhatSPAM The origin of SPAM
*
*       At the beginning of this project there was only a NXT unit that
*       had no heart nor build quality. It couldn't go straight due to its
*       bad construction. So I've asked GODS (The "Grande Operatore
*       Didattico Simonini") to study and then help me realize a NXT unit
*       that could "Do the impossible, Break The Unbreakable". And so the
*       SPAM ("Super Powerful Automated Mecha") was born. After a few tries
*       and after lots of tests we realized that is was "Not-So-Powerful".
*       So we maintained its name, but changed its meaning.
*       Now you will know that when we talk about SPAM we refer to a
*       "Semi-Powerful Automated Mecha".
*
*   \section Thanks Thank Yous and Other Stuff
*       
*       This project wouldn't have been possible with the help and the
*       presence of lots of people (Thank you all, guys!). However some
*       deserve the spotlight:
*       
*       - Ivan "GODS" Simonini: SPAM "Biological Father", as some people
*       call him, Metal Gear Solid fan and Mecha addicted.
*       - Giovanni "Simgi Dacav" Simoni: Programming Samurai and
*       predecessor in this "Crusade" against badly written software.
*       - Gabriele "Ranger" De Pedri: Benevolent Friend, Taiji Teacher and
*       "Saint".
*       - Chiara Finessi: The one who keeps up with all of the
*       "Non-Scientific" stuff I come up with.
*       - Michele "Master Miguèl" Graffeo: Cognitive Sciences researcher,
*       drinking buddy and the best RPG Master I've ever met.
*       - The Einherjar "Core": You know who you are XD. However, Ivan
*       Candiolli should be listed here. Thank you.
*/

/** @defgroup BROFistServer
 * 
 * This is the heart of the whole system.
 * 
 * It provides the user with a
 * communication server that will accept communication from any kind of
 * correctly configured client, send the data to a SPAM and then provide
 * feedback for the requests to the connected client.
 *
 */

/**     @defgroup BroBT BROFist Bluetooth Control
 *      @ingroup BROFistServer
 *
 *      This module controls the BlueTooth communication with the SPAM
 *      paired with the server.
 *
 *      We use @c LibBluez to communicate via BlueTooth, please refer to
 *      "An introduction to BlueTooth Programming" (
 *      http://people.csail.mit.edu/albert/bluez-intro/) to get an idea on
 *      how @c LibBluez works.
 */

/**     @defgroup BroComm BROFist Internal Communication Protocol
 *      @ingroup BROFistServer  
 *
 *      This module controls the internal communication (the one done via
 *      internal sockets).
 *      This communication may be between the BROFist server and SciCos
 *      blocks or another "ad hoc" client. It is achieved via the usage of
 *      UNIX sockets and some other powerful tricks. Please refer to the
 *      man page of @c socket.
 */

/**     @defgroup BroFist BROFist Generic Data Types and Defines
 *      @ingroup BROFistServer  
 *
 *      This module declares the common data types for the project.
 *
 *      It also defines the values of the various operations implemented in
 *      BROFist and the encoding for the 7 NXT ports.
 *
 *      \section BroOperations Implemented Operations:
 *
 *          There are 8 types of operations, the first 6 are used to
 *          request the values of sensors:
 *          - @c LIGHT_SENSOR: Used to request the value of a Light Sensor.
 *          The sensor's  value will be a number spanning from 0 to 1023.
 *          However, every kind of test highlighted that the "realistic"
 *          values will span between 400 and 700. This is due to the
 *          vicinity of the led to the sensor.
 *          - @c TOUCH_SENSOR: Used to request the value of a Touch Sensor.
 *          The sensor's  value will be either 1, if the sensor is "touching"
 *          something, or 0, if it's not "touching" anything.
 *          - @c SOUND_SENSOR: Used to request the value of a Sound Sensor.
 *          The sensor's value will be a number spanning from 0 to 1023,
 *          the lower the value, the higher the sound.
 *          - @c RADAR_SENSOR: Used to request the value of an Ultrasonic
 *          Sensor for Distance. The sensor's value will be a number
 *          spanning from 1 to 255, which are the centimeters of distance
 *          between the sensor and an obstacle in front of it.
 *          - @c TACHO_COUNT: Used to request the value of the revolution
 *          count in degree of a Servo Motor.
 *          - @c AVG_SPEED: Used to request the average speed in Degree
 *          per second of a Servo Motor.
 *
 *          The last 2 are used to set the speed or the power for Servo
 *          Motors:
 *          - @c SET_POWER: Used to set the RAW power for a Servo Motor.
 *          - @c SET_SPEED: Used to set the Speed in Degree per Second for
 *          a Servo Motor. This speed is controlled by the PID Controller
 *          implemented inside the Client for the SPAM. For this, please
 *          refer to the update_powers() function.
 *          
 *          \subsection FutureImp Future Improvements
 *          
 *              Since the nxtOSEK team sometimes releases new features the
 *              system is open for new implementations, such as the RS485
 *              control. BROFists can be easily improved by adding a new
 *              operation and then modifying the decode_bro_input()
 *              function in spam/BRO_spam_fist.c file.
 *
 *      \section BroFistStruct The TRUE BROFists
 *
 *          The BROfist system uses defined structures to send operations
 *          and ports to the SPAM. This is useful since it eases the
 *          implementations of future features, since every function that
 *          works with Fists uses methods to dinamically update its value
 *          based on the dimension of a bro_fist_t.
 */

/**     @defgroup BROOpts BROFist Options Manager
 *      @ingroup BROFistServer  
 *
 *      This module controls the options for the BROFist server.
 *
 *      It makes full use of the @c getopt library.
 */

/** @defgroup BROSpam BROFist SPAM Client
 *
 *      This is the BROFist Client to upload on a SPAM unit.
 *
 *      It receives the Operations requested, behaves accordingly and then
 *      sends the response data back to the Server.
 *      It also has a \ref PIDController that can make the 
 *      Servo Motors rotate at a certain speed.
 */

/**     @defgroup BROSClient Generic SPAM Data Types and Defines
 *      @ingroup BROSpam
 *
 *          This module defines some common data types for the SPAM Client
 *
 *          These modules are used to control every Servo Motor that might
 *          be installed on a @ref SPAM. As you might see, every function
 *          that interacts with a Servo Motor uses these structures, so if
 *          some modifications are needed they should be seameless.
 *
 *          @section BROSDefinitions BROFist SPAM Client Defines
 *             
 *              There also are a great deal of definitions in this module.
 *              They are needed to control the motors (There is a section
 *              dedicated to the PID Controller, as you might see by
 *              looking at the sources) and to initialize the Light Sensor
 *              and the Ultrasonic Distance Sensor. It's a shame that we
 *              can't activate them via Bluetooth and, instead, have to
 *              activate them by putting the correct ports in this file.
 */

/**     @defgroup BROSFists SPAM's BROFists Decoder
 *      @ingroup BROSpam
 *
 *          This module decodes the incoming BROFists and behaves accordingly
 *
 *          Every kind of Operation gets done by this module using the
 *          defines and structures in the bro_fist.h header file.
 */

/**     @defgroup BROSPid PID Controller Functions
 *      @ingroup BROSpam
 *
 *          This modules provides a PID Controller and an average speed function
 *
 *          There also are some general purpose functions to update the
 *          Servo Motors' speeds and to compute their average speed.
 *
 *          \section PIDController PID Controller
 *
 *              Directly from Wikipedia (http://en.wikipedia.org/wiki/PID_controller):
 *
 *              A proportional–integral–derivative controller (PID
 *              controller) is a generic control loop feedback mechanism
 *              (controller) widely used in industrial control systems  – a
 *              PID is the most commonly used feedback controller. A PID
 *              controller calculates an "error" value as the difference
 *              between a measured process variable and a desired setpoint.
 *              The controller attempts to minimize the error by adjusting
 *              the process control inputs. In the absence of knowledge of
 *              the underlying process, a PID controller is the best
 *              controller. However, for best performance, the PID
 *              parameters used in the calculation must be tuned according
 *              to the nature of the system – while the design is generic,
 *              the parameters depend on the specific system.
 *
 *              All the data needed by the PID is stored inside the motor_t
 *              related to the Servo Motor we want to control, such as the
 *              last three computed errors or the last powers used.
 *              The values used to tune the PID are stored inside
 *              @c BRO_spam_pid.c as defines.
 *
 *          \section BROSAvgSpd BROFist SPAM Average Speed Computation
 *
 *              Since the Servo Motors can only sense a change of rotation
 *              of a degree or more, computing the average speed in Degree
 *              per second can be a long, for embedded control, task, since
 *              the shorter the sampling time, the higher the computed
 *              error will be. E.g.: If we use a 2ms sampling time we would
 *              have to multiply the value by 500ms to get the average
 *              Degree per Second rotation speed. This wouldn't be a
 *              problem if the Tachometric Sensor embedded in the Servo
 *              Motors had a rational resolution, but in our case the error
 *              would be as high as 500 Degree per Second (since we can
 *              have an error of 1 degree every 2 ms). For that purpose and
 *              average speed computing function avg() has been
 *              implemented.
 */

/** @defgroup BROSciCos BROFist SciCos Interface Blocks
 *
 *      Interface blocks for SciCos written in C for communication with BROFist
 *
 *      The idea behind these blocks is simple: We use one block to control
 *      the socket, then various blocks to encode the requests. These
 *      requests and the socket descriptor (sent to other blocks as a
 *      standard integer value) are sent to various blocks that either send
 *      requests for sensors or send values to set the Speed or the Power
 *      for the Servo Motors.
 *      This kind of architecture is useful in case of new features
 *      implemented either on the \ref SPAM Client or on the BROFist Server.
 *      However, an example file for SciCos has been provided under the
 *      @c scicos folder: @c Comm_Test.cos
 *
 *      @section SimilOctave The understandability of the SciCosLab programming language
 *      
 *          You might want to browse throught the @c .sci files and this is
 *          a comprehensible thing to do, the problem is that the
 *          "Programming", or should I say Scripting, Language used to
 *          interact with SciCosLab is poorly documented and almost ignored
 *          throught all the Internet. However, feel free to use those
 *          files to understand the language via the "Retro-Engineering".
 *          Another great idea is to see how the provided blocks in
 *          SciCosLab were written.
 */

/** @defgroup BROCosComm BROFist SciCos Communication Blocks
 *  @ingroup BROSciCos
 *     
 *      This file contains all the function used to Control
 *      Communication between SciCos and the Server.
 *
 *      The general idea here is to have a set of funcions that controls
 *      the initialization and finalization for the communications via UNIX
 *      socket, another set to control the Dispatch of Fists to the SPAM
 *      Client for the request of a set of maxinum 7 sensors and the last
 *      one to dispatch the values for the Servo Motors.
 *
 *      \section Why7 Why only 7 requests?
 *
 *          Some people might ask why there are seven requests on the
 *          sensor dispatching block. This has been simply chosen because
 *          there are 7 ports on an NXT 2.0, included the Servo Motors'
 *          ones. You could, in fact, request both the average speed AND
 *          the Tachometric count for each motor and four other sensors'
 *          values. That really is the limit case, so the ports have been
 *          set to seven because that would be the "standard" limit case.
 *          If you ever need more than three values for the Servo Motors it
 *          would be wise to use a Sensor Dispatcher for the Sensors and a
 *          Sensor Dispatcher for the Servo Motors, just to have the same
 *          value for the same moment for every motor.
 */
        
/** @defgroup BROCosCommContr BROFist SciCos Communication Controller
 *  @ingroup BROCosComm
 *
 *      Functions needed to control the communications
 *      between SciCos and the BroFist Server.
 *
 *      By the way by "control" is intended the initialization and
 *      the finalization of the communications.
 *      These are used by a Communication Control Block that has
 *      only one Output Port: the Socket Descriptor. This Socket
 *      Descriptor have to be passed to each "Dispatcher" Block as
 *      the first input port.
 */

/** @defgroup BROCosSensDispatcher BROFist SciCos Sensor Dispatcher
 *  @ingroup BROCosComm
 *
 *      Funtions used to send and receive sensor packets between SciCos
 *      and the BROFist Server.
 */

/** @defgroup BROCosMotDispatcher BROFist SciCos Servo Motor Dispatcher
 *  @ingroup BROCosComm
 *
 *      Functions used to send and receive motor packets between
 *      SciCos and the BROFist Server.
 */

/** @defgroup BROCosEnc BROFist SciCos Fist Encoder Blocks
 *  @ingroup BROSciCos
 *  
 *      Blocks for the encooding of requests.
 *
 *      It is simply an addon to ease the lives of the users. They are
 *      blocks that need to be configurated correctly by entering the
 *      wanted Operation and Port.
 *      Since the SciCosLab language used to define how a Block works is a
 *      kind of programming language the block itself checks if the values
 *      used to set the BROFist are consistent. To understand more about
 *      this please browse throught @c bro_sens_enc.sci or @c
 *      bro_motor_enc.sci.
 */

/** @defgroup BROCosSensEnc BROFist SciCos Sensor Encoder
 *  @ingroup BROCosEnc
 *
 *  Interface functions used to encode requests for any Sensor.
 *
 *  These simply have to be configured inside the SciCos Graphical
 *  Interface by setting the Port and the Sensor wanted. 
 *  The values to use for the Sensors are:
 *      - @1 : Tachometric Counter
 *      - @2 : Average Speed
 *      - @3 : Light Sensor
 *      - @4 : Touch Sensor
 *      - @5 : Sound Sensor
 *      - @6 : Ultrasonic Distance Sensor
 *
 *  While the Port values are @1 to @4 for the four Sensors' Ports, and
 *  from @5 to @7 for the Motors' Ports.
 *  The block itself has a system to check if the user input the values is
 *  a coherent way.
 */

/** @defgroup BROCosMotEnc BROFist SciCos Servo Motor Encoder
 *  @ingroup BROCosEnc
 *
 *  Interface functions used to encode requests for any Servo Motor.
 */
