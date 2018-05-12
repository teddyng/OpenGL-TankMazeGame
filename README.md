# OpenGL-TankMazeGame

An OpenGL project for COMPUTER VISION & GRAPHICS (EEE2041), University of Surrey Electronic Engineering with Computer Systems 2016-2017

If you find this useful, please visit my LinkedIn page (https://www.linkedin.com/in/teddyng/) to endorsement my skills. To learn about what else I do, please visit my website at http://teddyng.co.uk.

Reminder: Plagiarism is a serious offense in force by the university, please refer to this code with care and at your own risk.

## Introduction

If you are visiting this page because you are also doing the same assessment, start early because this is probably the best and the most enjoyable coursework you will ever do. I upload this code to Github because at the time when I started this project, I couldn't find anything similar online to help me with (you might find some by older students, but they would be using an older version of OpenGL, so a lot of codes and libraries are pointing the wrong way, basiclly useless to me), and I think it would be useful if you can have an idea of how the code might look.

How I completed this project, I am a fairly bad programming, and I have no idea what OpenGL is, I still have to search up how to make a switch case and how to read from files, this is how bad I was in programming. This project had a timeframe of about 2-3 months, I didn't start untill the weekend before the deadline because I had no idea about what to do. My deadline was Monday and I started on Saturday, thinking I need to pass, attendance is 20% already, I just need to model a tank and some crate and I will have passed. Unfortunately, I only turn up to labs to sign the attendance sheet and left after 10 minutes, so I have absolutely no idea about OpenGL. So my 48 hours hackathon begins.

## The Task

"Your task is to develop a 3rd person maze action game using OpenGL. The goal of the game is to navigate a tank around a maze using the keyboard and mouse and eliminate all targets within a time limit while avoiding traps and falling off the edges. An outline of the functionality you are required to implement is given below. Marks will be awarded for providing additional functionality and for ease of user interaction."


## Timeline of my programming / 48 hours hackathon

**Saturday**

2pm - arrived at Lab, go through all tutorial, which I end up playing a game called 'FTL' on my laptop for 3 hours, until 6pm

6pm - Coursemate 1 suggested that I can use some of the code from tutorial to put into my code, would she found to be very useful, and she went home

1am - I made a cube and a coin inside the box, all with just the box texture applied to everything, I had no idea how to slove it after staring at it for a long time and there's no one in the labs to help me, so I gave up and went home.


**Sunday**

11am - I need to make the tank to get at least 30%, so I model the tank with the same code as the cube, but I forgot to make a new mash

3pm - Coursemate 1 arrived to help debug the mash problem for me, which turns out to be simply int mash 1 and mash 2...

4pm - Now I got a tank, a box and a coin, but all at the same point, I need to learn how to translate, coursemate 2 explained it to me, which lucky the function is already provided (Matrix.ccp).

5pm - I got a maze map and a tank, I passed!!! But I still have time, I might as well continue on. [SEE PICTURE 1]

6pm - I got the tank to move forward, back and sliding sideways. I've also got the camera to follow the tank. [SEE Video 1] 

10pm - The tank can now turn, but still can't collect coins or fall down the side of the box/crate [SEE Video 2]

12am - Worked out how to collect the coins (Array value changes when it is in the box), same method for falling, but the tank is scaled to ~0, so it disappear.

12am - Coursemate 3 missed the last bus back to Manor Park, so he explained about the head up display, it turns out to be very easy, because you just need to copy it from one of the tutorial. (I should have been the tutorial and not play games)

2am- Everything is working now!!! apart from shooting

3am - Too tried, so I went home and sleep


**Monday**

10am - Went to lecture with my laptop to continue my work with the help of coursemate 3, he helped me to set up xcode on my MacBook pro to work with OpenGL, I continue to do minor changes. Coursemate 4 copied some of my idea into his, but he did gave me some support throughout the weekend over what's app, except he reply 'I will check for you later' everytime

12pm - Went back to labs with coursemate 5, compared my game against hers, her game got physics and shooting

1pm - Cycle to McDonalds to use my Monopoly voucher as it was the deadline for it, got a chicken nuggets meal with orange juice and a coke for free, sat there for 15 minutes to eat my meal and I came up with the idea of a help menu

2pm - Back to labs to make the menu, and trying to make shooting work

4pm - Demonstrator and supervisor arrives, demonstrator helped me with shooting, turns out I just have an resetting loop, so the ball doesn't shoot, changed that quickly, probably still have 20 minutes until it is my turn for marking.

5pm - Is now my turn, 'make' the programme and I got a systematic error, he went on to the next guy when I try to sort it, I 'make clear' and 'qmake' and 'make', is working, I am so happy. Final mark is 87%

## Addition features that would be good to have

- A realistic background (a sky with clouds) and not just a plain colour
- Ground or sea beneath or around the boxes
- Physics to the tank movement
- Sound effects
- Game menu

## How to run my program

Linux: Download Model, commom and tank folder, open terminal in the tank folder, type ./tank

Mac Xcode: Download Model, commom and tank folder, google how to run openGL in Xcode, run the program 


## What I have learned

It has been a very hard journey and compared to a hackathon, there's no free food, no drinks... I didnt have time for implementing physics, but it has been the best two days of my life, I learned OpenGL and C++ in two days. I am so confident by Monday, I was helping others to their problems. And lastly a big thank you to coursemate 1-5, without their help, I would not have got to this point.

This was written about 1 month after the hand in day, all the timing are reconstructed from memory, Facebook and What's App records. My time management strategies are not encouraged.

If you need any help, pop me a message on Facebook, I am Teddy Ng and this is an introduction to OpenGL.
