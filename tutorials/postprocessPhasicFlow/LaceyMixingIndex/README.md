# Postprocessing to calculate Lecey mixing index in a rotating drum (v-1.0)

## Problem definition

The problem is to simulate a rotating drum with the diameter 0.24 m and the length 0.1 m rotating at 11.6 rpm. It is filled with 30,240 4-mm spherical particles. Particles are grouped into two types to measure the mixing dynamics in the drum. Lacey mixing index is used to evaluate the mixing quality.

Lacey mixing index is defined as:
$$LMI = \frac{σ^2_{max} - σ^2}{σ^2_{max} - σ^2_{min}}$$

where $σ^2$ is the variance of the concentration/fraction of one type of particles in sample volumes taken from the drum at a given time, $σ^2_{min}$ is the minimum variance corresponding to the completely mixed state, and $σ^2_{max}$ is the maximum variance corresponding to the completely segregated state. LMI near 0 shows a complete segregated bed, while LMI near 1 shows a completely mixed bed.

<div align="center">
<b>
    
A view of rotating drum
</b>
<b>

![](./LaceyMixingIndex.png)

</b></div>

***

## Setting up the case

Many of the settings in this simulation is similar to the tutorial [rotating drum small](../../sphereGranFlow/rotatingDrumSmall/README.md). Here, we only explain the differences.

### Creating particles with two types for mixing study

Let's start with caseSetup/shapes file.

<div align="center"> 
in <b>caseSetup/shapes</b> file
</div>

```C++
names       (sphere1 sphere2);                           // names of shapes 

diameters     (0.004 0.004);                             // diameter of shapes 

materials     (prop1 prop1);                             // material names for shapes
```

it defines two particles (`sphere1` and `sphere2`) with the same size and properites. The `shapeIndex` for `sphere1` is 0 and for `sphere2` is 1. So, in the mixing quality evaluations, particles with `shapeIndex` of 0 refer to `sphere1` and particles with `shapeIndex` of 1, refer to `sphere2`.  

Open the file  `settings/particlesDict`. Two dictionaries, `positionParticles` and `setFields` position particles and set the field values for the particles. 
In dictionary `positionParticles`, the positioning `method` is `ordered`, which position particles in order in the space defined by `box`. `box` space is defined by two corner points `min` and `max`. In dictionary `orderedInfo`, `numPoints` defines number of particles; `distance`, the distance between two adjacent particles, and `axisOrder` defines the axis order for filling the space by particles.

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
positionParticles                                // positions particles 
{
    method ordered;                              // other options: random and empty

    mortonSorting              Yes;              // perform initial sorting based on morton code?   

    orderedInfo
    {
        distance  0.004;                         // minimum space between centers of particles

        numPoints 30240;                         // number of particles in the simulation 

        axisOrder (z x y);                       // axis order for filling the space with particles
    }

    regionType box;                              // other options: cylinder and sphere  

    boxInfo                                      // box information for positioning particles 
    {
        min (-0.08 -0.08 0.015);                 // lower corner point of the box 

        max ( 0.08  0.08 0.098);                 // upper corner point of the box 
    }
}
```

In dictionary `setFields`, dictionary `defaultValue` defines the initial value for all particles fields (here, `velocity`, `acceleration`, `rVelocity`, and `shapeName`). Note that `shapeName` field should be consistent with the names of shapes that you set in `caseSetup/shapes` file. In the `selectors` part, in the `shapeAssigne` sub-dictionary, a box that encompass half of the drum (left side of the drum) is defined and all the particles inside that box are selected. The `shapeName` for all the selected particles is set to `sphere2`. In this way, half of the particles in the left side are of type `sphere2` and the other half in the right side of the drum are of type `sphere1`.

<div align="center"> 
in <b>settings/particlesDict</b> file
</div>

```C++
setFields
{
    /*
        Default value for fields defined for particles
        These fields should always be defined for simulations with 
        spherical particles.
    */
    defaultValue 
    {
        velocity        realx3  (0 0 0);         // linear velocity (m/s)
        acceleration    realx3  (0 0 0);         // linear acceleration (m/s2)
        rVelocity       realx3  (0 0 0);         // rotational velocity (rad/s)
        shapeName       word    sphere1;         // name of the particle shape 
    }

    selectors
    {
        shapeAssigne
        {
            selector           box;     // other options: cylinder, sphere, randomPoints 
            // select particles in the half of the drum to set to sphere2
            boxInfo
            {
                min (-0.1 -0.1 0.0);
                max ( 0.0  0.1 0.1);
            }

            fieldValue                           // fields that the selector is applied to 
            {
                shapeName   word sphere2;       // sets shapeName of the selected points to sphere2
            }
        }
    }
}
```

Enter the following command in the terminal to create the particles and store them in `0` folder.

`> particlesPhasicFlow`

### Setting up Lacey mixing index parameters

Lacey mixing index calculation is a post-processing operation. To set it up, we need to provide the file `postprocessDataDict` under `settings` folder. For more information on postprocessData module in PhasicFlow, [click here](./../../../src/PostprocessData/readme.md). 

In the `mixingInedexCalc` dictionary, `processMethod` is set to `mixingIndex` and the `processRegion` is set to `rectMesh`. `processRegion` represents a rectangular mesh whose cells are used for sampling points for evaluating the mixing index. The information of the rectMesh is given in `rectMeshInfo` sub-dictionary. `indexType` is set to `Lacey`, and information for calculating the Lacey mixing index is given in `LaceyInfo` sub-dictionary. In this sub-dictionary, `threshold` defines a minimum limit for number of particles in each sample (cell) to be included in the mixing index calculations. `type1Frac` is set to 0.5, which is equal to number fraction (concentration) of type1 particles in the drum. `type1Selection` specifies how to identify type1 particles. Here, it is set to `equal`, which means that type1 particles are those whose `shapeIndex` field is equal to 0 (as defined in `caseSetup/shapes` file). The `equalInfo` sub-dictionary provides more information for this selection.

<div align="center"> 
in <b>settings/postprocessDataDict</b> file
</div>

```C++
runTimeActive       yes;

shapeType           sphere;

defaultTimeControl
{
    timeControl        simulationTime;
    startTime          0;
    executionInterval  0.5; 
}

components
(
    mixingInedexCalc
    {
        processMethod   mixingIndex;
        
        processRegion   rectMesh;
        
        indexType       Lacey;
        
        timeControl     default;

        precision 4;

        scientific no;

        rectMeshInfo 
        {
            min (-0.12 -0.12 0.00); // lower corner point of the box 
            max (0.12   0.12 0.11); // upper corner point of the box
            
            nx 15; // number of divisions in x direction
            ny 15; // number of divisions in y direction
            nz 8; // number of divisions in z direction 
        }

        LaceyInfo
        {
            type1Frac      0.5;
            threshold      20;
            type1Selection equal;

            equalInfo
            {
                field   shapeIndex;
                value   0;
            }
        }
        
    }

);
```

## Running the case

The solver for this simulation is `sphereGranFlow`. Enter the following command in the terminal. Depending on the computational power, it may take a few minutes to a few hours to complete. 

`> sphereGranFlow`

The results of the post-processing are stored under folder `postprocessData` in the root folder of the simulation.