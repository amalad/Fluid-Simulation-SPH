# Introduction
Realistic fluid animations are one of the most important phenomena in computer graphics. They are frequently used in computer gaming and visual effects industries. Fluids, in particular liquids such as water, are responsible for many visually rich phenomena, and simulating them has been an area of long-standing interest and challenge in computer graphics. Several approaches have been developed to this end, but in this project we focus on one of the most popular Lagrangian approach to fluid simulation: Smoothed Particle Hydrodynamics (SPH), specifically Muller's [2003] work on interactve fluid simulation.

SPH is an interpolation method for particle systems that distributes quantities in a local neighborhood of each particle using radial symmetrical smoothing kernels. According to SPH, a scalar quantity A is interpolated at location <b>r</b> by a weighted sum of contributions from all particles:

<p style="text-align:justify;"> A<sub>S</sub>(<b>r</b>) = &sum;<sub>j</sub>m<sub>j</sub>(A<sub>j</sub>/&#961;<sub>j</sub>)W(<b>r</b>-<b>r</b><sub>j</sub>, h)</p>

Where mj is the mass of the particle j, r<sub>j</sub> is its position, ρ<sub>j</sub> is its density and A<sub>j</sub> the field quantity at r<sub>j</sub>.The mass term remains constant throughout the entire simulation but the density term keeps changing and has to be recalculated in each iteration. By substitution, we get the density at location <b>r</b> as:

<p style="text-align:justify;">&#961;<sub>S</sub>(<b>r</b>) = &sum;<sub>j</sub>m<sub>j</sub>W(<b>r</b>-<b>r</b><sub>j</sub>, h)</p> 

Furthermore, the gradient and Laplacian of A are simply:

<p style="text-align:justify;">&nabla;A<sub>S</sub>(<b>r</b>) = &sum;<sub>j</sub>m<sub>j</sub>(A<sub>j</sub>/&#961;<sub>j</sub>)&nabla;W(<b>r</b>-<b>r</b><sub>j</sub>, h)&nbsp;&nbsp;&nbsp;and</p>
<p style="text-align:justify;">&nabla;<sup>2</sup>A<sub>S</sub>(<b>r</b>) = &sum;<sub>j</sub>m<sub>j</sub>(A<sub>j</sub>/&#961;<sub>j</sub>)&nabla;<sup>2</sup>W(<b>r</b>-<b>r</b><sub>j</sub>, h)</p>

# Particle Based Fluid Simulation
## Smoothes Particle Hydrodynamics
The use of particles instead of a stationary grid inherently accounts for the conservation of mass since the number of particles is fixed and the mass of each particle remains the same. Moreover, the substantial derivative of the velocity field in the Navier-Stokes equation can is reduced to the time derivative of the velocity of the particles, since the particles move with the fluid. This means that the convective term v·∇v is not needed for particle systems. The resultant equation is:

<p style="text-align:justify;">&#961;(dv/dt) = -&nabla;p + &#961;<b>g</b> + &mu;&nabla;<sup>2</sup><b>v</b></p>

where the three force density fields on the RHS model pressure, external forces and viscosity, respectively. The acceleration of the particle i is then:

<p style="text-align:justify;"><b>a</b><sub>i</sub> = d<b>v</b><sub>i</sub>/dt = <b>f</b><sub>i</sub>/&rho;<sub>i</sub></p>

## The Pressure Term
The application of SPH to the pressure field while accounting for symmetry yields the folowing equation:

<p style="text-align:justify;"><b>f</b><sup>pressure</sup><sub>i</sub> = -&nabla;p(<b>r</b><sub>i</sub>) = -&sum;<sub>j</sub>m<sub>j</sub>(p<sub>i</sub>+p<sub>j</sub>)/2&rho;<sub>j</sub>&nabla;W(<b>r</b><sub>j</sub>-<b>r</b><sub>j</sub>,h)</p>

Where the pressure term is simply p<sub>j</sub> = K(ρ<sub>j</sub>-ρ<sub>0</sub>), K being the gas constant.

## The Viscosity Term
The application of SPH to the viscosity field while accounting for symmetry yields the folowing equation:

<p style="text-align:justify;"><b>f</b><sup>viscosity</sup><sub>i</sub> = &mu;&sum;<sub>j</sub>m<sub>j</sub>(<b>v</b><sub>j</sub>-<b>v</b><sub>i</sub>)/&rho;<sub>j</sub>&nabla;W(<b>r</b><sub>j</sub>-<b>r</b><sub>j</sub>,h)</p>

## Surface Tension
The surface of the liquid can be found using an additional field quantity called the colour field. The smoothed colour field is as follows:

<p style="text-align:justify;">c<sub>S</sub> = &sum;<sub>j</sub>m<sub>j</sub>(1/&rho;<sub>j</sub>)W(<b>r</b>-<b>r</b><sub>j</sub>, h)</p>

The gradient of this field gives us the surface normal pointing into the fluid. Let this vector be n. Then the force density acting near the surface is:
<p style="text-align:justify;">f<sup>surface</sup> = &sigma;&nabla;<sup>2</sup>c<sub>S</sub><b>n</b>/|<b>n</b>|</p>

## Smoothing Kernels
The authors use the following kernel functions in their paper: W<sub>poly6</sub> for density and surface tension force calculatons, W<sub>spiky</sub> for pressure force calculations and W<sub>viscosity</sub> for viscosity force calculations. A detailed formulaion can be found in their paper.

# Discussion
The kernels used in the original paper yielded a somewhat unstable system. Hence, we had to make a few tweaks to the original algorithm to yield satisfactory results. We used slighty modified versions of the original kernel functions and discarded the surface tension forces in our implementations. We also observed that the velocities eventually shot up after multiple collisions, probably because our system used a minimum timestep for its computations. To handle this problem, we clamped the velocities of the particles in both the 2D and 3D systems. Unfortunately, we couldn't simulate more than a few hundred particles on our system at a go due to limitations on the available computational resources. We tested two integration techniques for velocity and position calculations - Leap Frog and Forward Euler. Surprisingly, the forward Euler technique gave better results than the former, with the former spiralling down to an unstable system faster when the velocities are not clamped.

# Results
Following are a few images of our simulations:

## The 2D Case
<img src="gifs/FluidSim2D.gif" width="400px" />

## The 3D Case
<img src="gifs/FluidSim3D.gif" width="400px" />
