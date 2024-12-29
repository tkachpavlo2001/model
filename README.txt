ACS_MODEL




Description:
This application provides an interactive approach to acquire hands-on skills in the experimental tuning of PID controllers. 
Also, it facilitates users in evaluating the effectiveness of the automated control system by providing the opportunity to run and estimate the model with the regulator excluded.
Even a user without technical education is capable of understanding basics within half an hour with the explanations provided by those educated in this field, as has been observed.

Automated control system in this application is based on the electromotive drive as the controlled object. The automated control system is dedicated to sustaining a certain angular displacement value of the engine shaft. The regulating value is the voltage applied to the motor terminals. The electric motor is a direct current engine with independent windings. Consequently, the source of electricity is a DC type.

There are two ways the model can be run:
1. The model in full completion.
2. The model with no PID controller. 
The second way has two different modes that differ in the value fixed as the output from the system. One of them, the same as in the main mode with full completion, is the angular displacement, and the second one is the angular velocity.
Therefore, here are three modes of the model run as:
1. Full completion.
2. Without PID controller and angular displacement fixation.
3. Without PID controller and angular velocity fixation.

The main application window allows a user to choose the mode.

When the mode has been chosen, two windows are provided for use. The chart window plots data in real-time, and the sliders allow a user to change the percentage of the main values applied to the model. These main values are defined in another window. This window is intended to define the main values for the run. Their definition serves as a limitation because the chart window provides the ability to use the percentage range only within the 0-100% span.

The value definition window defines the following values for each mode:
1. Input value
2. Torque coefficients
In full completion mode, a reference value is used as the input value, while in other modes, a voltage is used as the input value. The torque coefficients have the same 'k0' and 'k1' in every mode, and they correspond to the equation:
'T(w) = k0 + k1 * w'
, where 'w' is angular velocity;
'k0' is the constant part;
'k1' is proportionally related to angular velocity part.

The full completion mode requires the definition of the 'kp', 'ki' and 'kd' values that correspond to the equation:
'u(t) = kp * e(t) + ki * integral( e(t), dt ) + kd * derivative( e(t), dt)'
, where 'kp' is the proportional coefficient;
'ki' is the integral coefficient;
'kd' is the derivative coefficient;
The regulator tuning consists of the finding appropriate values for these coefficients that satisfy the requirements.

The chart plot window allows a user to change input value, 'k0', and 'k1' with the sliders.

All changes can be applied in real time, while the model is processing and the chart is plotting. This opportunity allows for the implementation the interactive approach that has been mentioned.

The model list has been composed from the simpler models, easiest to visualise, to the more complex ones:
1. Without a PID controller and angular velocity fixation.
2. Without a PID controller and angular displacement fixation.
3. Full completion.
This order is designed to gradually introduce this concept with step-by-step increasing difficulty to understand and visualize.

In the last mode on the list, it is recommended not to use the next coefficients until a user has understood the concept of the previous.
For example, start with only 'kp'. When the user has understood its impact, he can start changing 'ki'. The last 'kd' coefficient can be used after the 'kp' and 'ki' regulation is clearly understandable to the user.

The theory must be under review while using this application for the best study experience.

The application was developed by Pavlo Tkach, a former student of the Faculty of Electrical Engineering at Kryvyi Rih National University. The application uses the program modules developed during the qualification work on the theme "Research of modern software methods of the classic PID controller improvement and modifications" for a master's degree in software engineering at the Faculty of Information Technologies at Kryvyi Rih National University.

The application is intended to be used by the Faculty of Electrical Engineering and anyone who is interested in PID controller concepts.




License:
This project is licensed under the GNU General Public License v3.0 - see the LICENSE.txt file for details.
https://github.com/tkachpavlo2001/model


