# ASCII Fluid Dinamics

After see this [amazing video](https://www.youtube.com/watch?v=QMYfkOtYYlg) and see this [repository](https://github.com/lorenzo-stoakes/ascii-fluids) i attempt to make my own version of ascii fluids dinamics.

My starter point has the original soruce [here](http://www.ioccc.org/2012/endoh1/hint.html) the same program used in video, also the lorenzo source helps a lot.

I try not use complex numbers (mostly cause i have much trouble to make them work propety with c++)

To understand better the concept you can check [SPH Smoothed Particle Hydrodynamics](https://en.wikipedia.org/wiki/Smoothed-particle_hydrodynamics)

# Compile
```g++ asciiFluidSimulation.cpp -o asciiFluidSimulation```

Then run:

```./asciiFluidSimulation < ./inputs/clock.txt```

# Examples

Clock:
<p align="center">
  <img src="https://github.com/Krymancer/ascii-fluids/blob/master/clock.gif?raw=true">
</p>
