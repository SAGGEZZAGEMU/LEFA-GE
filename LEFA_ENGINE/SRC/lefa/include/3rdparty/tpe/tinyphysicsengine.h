#ifndef _TINYPHYSICSENGINE_H
#define _TINYPHYSICSENGINE_H

/* Ported To C89 
PHIKILL | SILD TEAM
*/

/*
  tinyphysicsengine (TPE)

  Simple/suckless header-only hybrid 3D physics engine with no floating point,
  only 32 bit int arithmetic, similar to e.g. small3dlib.
  
  Conventions and formats are the same or similar to those of small3dlib so as
  to make them easily integrate with each other.

  The library works with bodies made of spheres connected by elastic springs,
  i.e. soft bodies which however behave as "stiff" bodies by default and can
  be used to fake rigid body physics as well. Bodies are placed in environemnts
  specified by a distance function that allows to implement any mathematical
  shape.

  Orientations/rotations are in extrinsic Euler angles in the ZXY order (by Z,
  then by X, then by Y), if not mentioned otherwise. Angles are in TPE_Units,
  TPE_FRACTIONS_PER_UNIT is full angle (2 PI). Sometimes rotations can also be
  specified in the "about axis" format: here the object is rotated CW by given
  axis by an angle that's specified by the magnitude of the vector.

  Where it matters (e.g. rotations about axes) we consider a left-handed coord.
  system (x right, y up, z forward).

  ------------------------------------------------------------------------------

  by drummyfish, 2022

  version 0.8d

  This work's goal is to never be encumbered by any exclusive intellectual
  property rights. The work is therefore provided under CC0 1.0
  (https://creativecommons.org/publicdomain/zero/1.0/) + additional WAIVER OF
  ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of intellectual property
  rights not already waived by CC0 1.0. The WAIVER OF ALL INTELLECTUAL PROPERTY
  RGHTS is as follows:

  Each contributor to this work agrees that they waive any exclusive rights,
  including but not limited to copyright, patents, trademark, trade dress,
  industrial design, plant varieties and trade secrets, to any and all ideas,
  concepts, processes, discoveries, improvements and inventions conceived,
  discovered, made, designed, researched or developed by the contributor either
  solely or jointly with others, which relate to this work or result from this
  work. Should any waiver of such right be judged legally invalid or
  ineffective under applicable law, the contributor hereby grants to each
  affected person a royalty-free, non transferable, non sublicensable, non
  exclusive, irrevocable and unconditional license to this right. 
*/

/* #include <stdint.h>
stdint is for c99 or more
*/

typedef int TPE_Unit;                 /* < Basic fixed point unit type. */
typedef short TPE_UnitReduced;        /* < Like TPE_Unit but saving space */

#define TPE_FRACTIONS_PER_UNIT 512      /* < one fixed point unit, don't change */
#define TPE_F TPE_FRACTIONS_PER_UNIT    /* < short for TPE_FRACTIONS_PER_UNIT */ 
#define TPE_JOINT_SIZE_MULTIPLIER 32    /* < joint size is scaled (size saving) */

#define TPE_INFINITY 2147483647

#define TPE_JOINT_SIZE(joint) ((joint).sizeDivided * TPE_JOINT_SIZE_MULTIPLIER)

#ifndef TPE_APPROXIMATE_LENGTH
  #define TPE_APPROXIMATE_LENGTH 0      /* < whether or not use length/distance 
                                           approximation rather than exact 
                                           calculation (1 is faster but less
                                           accurate), beware of possible lower
                                           stability */
#endif

#if !TPE_APPROXIMATE_LENGTH
  #define TPE_DISTANCE TPE_dist
  #define TPE_LENGTH TPE_vec3Len
#else
  #define TPE_DISTANCE TPE_distApprox
  #define TPE_LENGTH TPE_vec3LenApprox
#endif

#ifndef TPE_LOG
  #define TPE_LOG(s) ; /* redefine to some print function to show debug logs */
#endif

#ifndef TPE_LOW_SPEED
/* Speed, in TPE_Units per ticks, that is considered low (used e.g. for auto
  deactivation of bodies). 
*/
  #define TPE_LOW_SPEED 30
#endif

#ifndef TPE_RESHAPE_TENSION_LIMIT
/* Tension limit, in TPE_Units, after which a non-soft body will be reshaped.
  Smaller number will keep more stable shapes but will cost more performance. 
*/
  #define TPE_RESHAPE_TENSION_LIMIT 20
#endif

#ifndef TPE_RESHAPE_ITERATIONS
/* How many iterations of reshaping will be performed by the step function if
  the body's shape needs to be reshaped. Greater number will keep shapes more
  stable but will cost some performance. 
*/
  #define TPE_RESHAPE_ITERATIONS 3
#endif

#ifndef TPE_DEACTIVATE_AFTER
/** After how many ticks of low speed should a body be disabled. This mustn't
  be greater than 255. */
  #define TPE_DEACTIVATE_AFTER 128
#endif

#ifndef TPE_LIGHT_DEACTIVATION
/** When a body is activated by a collision, its deactivation counter will be
  set to this value, i.e. after a collision the body will be prone to deactivate
  sooner than normally. This is to handle situations with many bodies touching
  each other that would normally keep activating each other, never coming to
  rest. */
  #define TPE_LIGHT_DEACTIVATION \
    (TPE_DEACTIVATE_AFTER - TPE_DEACTIVATE_AFTER / 10)
#endif

#ifndef TPE_TENSION_ACCELERATION_DIVIDER
/** Number by which the base acceleration (TPE_FRACTIONS_PER_UNIT per tick
  squared) caused by the connection tension will be divided. This should be
  power of 2. */
  #define TPE_TENSION_ACCELERATION_DIVIDER 32
#endif

#ifndef TPE_TENSION_ACCELERATION_THRESHOLD
/** Limit within which acceleration caused by connection tension won't be
  applied. */
  #define TPE_TENSION_ACCELERATION_THRESHOLD 5
#endif

#ifndef TPE_TENSION_GREATER_ACCELERATION_THRESHOLD
/** Connection tension threshold after which twice as much acceleration will
  be applied. This helps prevent diverting joints that are "impaled" by
  environment.*/
  #define TPE_TENSION_GREATER_ACCELERATION_THRESHOLD \
    (TPE_TENSION_ACCELERATION_THRESHOLD * 3)
#endif

#ifndef TPE_COLLISION_RESOLUTION_ITERATIONS
/** Maximum number of iterations to try to uncollide two colliding bodies. */
  #define TPE_COLLISION_RESOLUTION_ITERATIONS 16
#endif

#ifndef TPE_COLLISION_RESOLUTION_MARGIN
/** Margin, in TPE_Units, by which a body will be shifted back to get out of
  collision. */
  #define TPE_COLLISION_RESOLUTION_MARGIN (TPE_F / 64)
#endif

#ifndef TPE_NONROTATING_COLLISION_RESOLVE_ATTEMPTS
/** Number of times a collision of nonrotating bodies with environment will be
  attempted to resolve. This probably won't have great performance implications
  as complex collisions of this kind should be relatively rare. */
  #define TPE_NONROTATING_COLLISION_RESOLVE_ATTEMPTS 8
#endif

#ifndef TPE_APPROXIMATE_NET_SPEED
/** Whether to use a fast approximation for calculating net speed of bodies
  which increases performance a bit. */
  #define TPE_APPROXIMATE_NET_SPEED 1
#endif

#define TPE_PRINTF_VEC3(v) printf("[%d %d %d]",(v).x,(v).y,(v).z);

typedef struct
{
  TPE_Unit x;
  TPE_Unit y;
  TPE_Unit z;
} TPE_Vec3;

/** Keeps given point within specified axis-aligned box. This can be used e.g.
  to smooth rendered movement of jittering physics bodies. */
TPE_Vec3 TPE_vec3KeepWithinBox(TPE_Vec3 point, TPE_Vec3 boxCenter,
  TPE_Vec3 boxMaxVect);

TPE_Vec3 TPE_vec3KeepWithinDistanceBand(TPE_Vec3 point, TPE_Vec3 center,
  TPE_Unit minDistance, TPE_Unit maxDistance);

TPE_Vec3 TPE_vec3(TPE_Unit x, TPE_Unit y, TPE_Unit z);
TPE_Vec3 TPE_vec3Minus(TPE_Vec3 v1, TPE_Vec3 v2);
TPE_Vec3 TPE_vec3Plus(TPE_Vec3 v1, TPE_Vec3 v2);
TPE_Vec3 TPE_vec3Cross(TPE_Vec3 v1, TPE_Vec3 v2);
TPE_Vec3 TPE_vec3Project(TPE_Vec3 v, TPE_Vec3 base);
TPE_Vec3 TPE_vec3ProjectNormalized(TPE_Vec3 v, TPE_Vec3 baseNormalized);
TPE_Vec3 TPE_vec3Times(TPE_Vec3 v, TPE_Unit units);
TPE_Vec3 TPE_vec3TimesPlain(TPE_Vec3 v, TPE_Unit q);
TPE_Vec3 TPE_vec3Normalized(TPE_Vec3 v);

TPE_Unit TPE_vec3Dot(TPE_Vec3 v1, TPE_Vec3 v2);
TPE_Unit TPE_vec3Len(TPE_Vec3 v);
TPE_Unit TPE_vec3LenApprox(TPE_Vec3 v);

/** Returns an angle in TPE_Units (see angle conventions) of a 2D vector with
  the X axis, CCW. */
TPE_Unit TPE_vec2Angle(TPE_Unit x, TPE_Unit y);

/** Keeps given value within specified range. This can be used e.g. for movement
  smoothing. */
TPE_Unit TPE_keepInRange(TPE_Unit x, TPE_Unit xMin, TPE_Unit xMax);

static TPE_Unit TPE_abs(TPE_Unit x);
static TPE_Unit TPE_max(TPE_Unit a, TPE_Unit b);
static TPE_Unit TPE_min(TPE_Unit a, TPE_Unit b);
static TPE_Unit TPE_nonZero(TPE_Unit x);
static TPE_Unit TPE_dist(TPE_Vec3 p1, TPE_Vec3 p2);
static TPE_Unit TPE_distApprox(TPE_Vec3 p1, TPE_Vec3 p2);
TPE_Unit TPE_sqrt(TPE_Unit x);

/** Compute sine, TPE_FRACTIONS_PER_UNIT as argument corresponds to 2 * PI
  radians. Returns a number from -TPE_FRACTIONS_PER_UNIT to
  TPE_FRACTIONS_PER_UNIT. */
TPE_Unit TPE_sin(TPE_Unit x);
TPE_Unit TPE_cos(TPE_Unit x);
TPE_Unit TPE_atan(TPE_Unit x);

typedef struct
{
  TPE_Vec3 position;
  TPE_UnitReduced velocity[3]; /* < not TPE_Vec3 to save size */
  unsigned char sizeDivided; /**< size (radius, ...), for saving space divided by 
                            TPE_JOINT_SIZE_MULTIPLIER */
} TPE_Joint;

typedef struct
{
  unsigned char joint1;
  unsigned char joint2;
  unsigned short length;     /* /< connection's preferred length, uint16_t saves space */
} TPE_Connection;

#define TPE_BODY_FLAG_DEACTIVATED 1    /**< Not being updated due to low energy,
                                            "sleeping", will be woken by
                                            collisions etc. */
#define TPE_BODY_FLAG_NONROTATING 2    /**< When set, the body won't rotate, 
                                            will only move linearly. Here the
                                            velocity of the body's first joint
                                            is the velocity of the whole
                                            body. */
#define TPE_BODY_FLAG_DISABLED 4       /**< Disabled, not taking part in
                                            simulation. */
#define TPE_BODY_FLAG_SOFT 8           /**< Soft connections, effort won't be
                                            made to keep the body's shape. */
#define TPE_BODY_FLAG_SIMPLE_CONN 16   /**< Simple connections, don't zero out
                                            antagonist forces or apply
                                            connection friction, can increase
                                            performance. */
#define TPE_BODY_FLAG_ALWAYS_ACTIVE 32 /**< Will never deactivate due to low
                                            energy. */

/** Function used for defining static environment, working similarly to an SDF
  (signed distance function). The parameters are: 3D point P, max distance D.
  The function should behave like this: if P is inside the solid environment
  volume, P will be returned; otherwise closest point (by Euclidean distance) to
  the solid environment volume from P will be returned, except for a case when
  this closest point would be further away than D, in which case any arbitrary
  point further away than D may be returned (this allows for optimizations). */
typedef TPE_Vec3 (*TPE_ClosestPointFunction)(TPE_Vec3, TPE_Unit);

/** Function that can be used as a joint-joint or joint-environment collision
  callback, parameters are following: body1 index, joint1 index, body2 index,
  joint2 index, collision world position. If body1 index is the same as body1
  index, then collision type is body-environment, otherwise it is body-body
  type. The function has to return either 1 if the collision is to be allowed
  or 0 if it is to be discarded. This can besides others be used to disable
  collisions between some bodies. */
typedef unsigned char(*TPE_CollisionCallback)(unsigned short, 
                                              unsigned short, 
                                              unsigned short, 
                                              unsigned short,
                                              TPE_Vec3);

/** Function used by the debug drawing functions to draw individual pixels to
  the screen. The parameters are following: pixel x, pixel y, pixel color. */
typedef void (*TPE_DebugDrawFunction)(unsigned short, 
                                      unsigned short, 
                                      unsigned char);

/** Physics body made of spheres (each of same weight but possibly different
  radia) connected by elastic springs. */
typedef struct
{
  TPE_Joint *joints;
  unsigned char jointCount;
  TPE_Connection *connections;
  unsigned char connectionCount;
  TPE_UnitReduced jointMass;       /* /< mass of a single joint   */
  TPE_UnitReduced friction;        /* /< friction  of each joint  */
  TPE_UnitReduced elasticity;      /* /< elasticity of each joint */
  unsigned char flags;
  unsigned char deactivateCount;
} TPE_Body;

typedef struct
{
  TPE_Body *bodies;
  unsigned short bodyCount;
  TPE_ClosestPointFunction environmentFunction;
  TPE_CollisionCallback collisionCallback;
} TPE_World;

/** Tests the mathematical validity of given closest point function (function
  representing the physics environment), i.e. whether for example approaching
  some closest point in a straight line keeps approximately the same closest
  point. Note that this function may take a long time to complete, especially
  with higher gridResolution values and more complex environment functions. You
  should use this function to test your environment function, especially if you
  create functions for your own shapes etc. The cornerFrom and cornerTo points
  are corners of an axis-aligned box within which testing will take place, 
  gridResolution defines numbers of points (i.e. step length) along each
  dimension to test (recommended e.g. 64), allowedError says error within which
  points will be considered the same (recommended range approx. 10 to 200). If
  testing is successful, 1 is returned, otherwise 0 is returned and the point
  around which error was detected is returned in errorPoint (unless the pointer
  is 0 in which case it is ignored). */
unsigned char TPE_testClosestPointFunction(TPE_ClosestPointFunction f,
  TPE_Vec3 cornerFrom, TPE_Vec3 cornerTo, unsigned char gridResolution,
  TPE_UnitReduced allowedError, TPE_Vec3 *errorPoint);

void TPE_bodyInit(TPE_Body *body, 
  TPE_Joint *joints, unsigned char jointCount, 
  TPE_Connection *connections, unsigned char connectionCount,
  TPE_Unit mass);

void TPE_worldInit(TPE_World *world,
  TPE_Body *bodies, unsigned short bodyCount,
  TPE_ClosestPointFunction environmentFunction);

/** Gets orientation (rotation) of a body from a position of three of its
  joints. The vector from joint1 to joint2 is considered the body's forward
  direction, the vector from joint1 to joint3 its right direction. The returned
  rotation is in Euler angles (see rotation conventions). */
TPE_Vec3 TPE_bodyGetRotation(const TPE_Body *body, unsigned short joint1, 
  unsigned short joint2, unsigned short joint3);

void TPE_vec3Normalize(TPE_Vec3 *v);

/** Rotates a 3D point by given Euler angle rotation (see rotation
  conventions). */
TPE_Vec3 TPE_pointRotate(TPE_Vec3 point, TPE_Vec3 rotation);

/** Returns an inverse rotation to given rotation, in Euler angles (see rotation
  conventions). */
TPE_Vec3 TPE_rotationInverse(TPE_Vec3 rotation);

/** Returns a connection tension, i.e. a signed percentage difference against
  desired length (TPE_FRACTIONS_PER_UNIT means 100%). */
static TPE_Unit TPE_connectionTension(TPE_Unit length,
  TPE_Unit desiredLength);

/** Rotates a rotation specified in Euler angles by given axis + angle (see
  rotation conventions). Returns a rotation in Eurler angles. */
TPE_Vec3 TPE_rotationRotateByAxis(TPE_Vec3 rotation, TPE_Vec3 rotationByAxis);

/** Computes the formula of a 1D collision of rigid bodies. */
void TPE_getVelocitiesAfterCollision(TPE_Unit *v1, TPE_Unit *v2, TPE_Unit m1,
  TPE_Unit m2, TPE_Unit elasticity);

/** Computes orientation/rotation (see docs for orientation format) from two
  vectors (which should be at least close to being perpendicular and do NOT
  need to be normalized). This can be used to determine orientation of a body
  from a relative position of its joints. */
TPE_Vec3 TPE_rotationFromVecs(TPE_Vec3 forward, TPE_Vec3 right);

TPE_Joint TPE_joint(TPE_Vec3 position, TPE_Unit size);

/** Mostly for internal use, resolves a potential collision of two joints in a
  way that keeps the joints outside provided environment (if the function
  pointer is not 0). Returns 1 if joints collided or 0 otherwise. */
unsigned char TPE_jointsResolveCollision(TPE_Joint *j1, TPE_Joint *j2,
  TPE_Unit mass1, TPE_Unit mass2, TPE_Unit elasticity, TPE_Unit friction,
  TPE_ClosestPointFunction env);

/** Mostly for internal use, tests and potentially resolves a collision between
  a joint and environment, returns 0 if no collision happened, 1 if it happened
  and was resolved normally and 2 if it couldn't be resolved normally. */
unsigned char TPE_jointEnvironmentResolveCollision(TPE_Joint *joint, TPE_Unit
  elasticity, TPE_Unit friction, TPE_ClosestPointFunction env);

/** Tests whether a body is currently colliding with the environment. */
unsigned char TPE_bodyEnvironmentCollide(const TPE_Body *body,
  TPE_ClosestPointFunction env);

/** Mostly for internal use, tests and potentially resolves a collision of a
  body with the environment, returns 1 if collision happened or 0 otherwise. */
unsigned char TPE_bodyEnvironmentResolveCollision(TPE_Body *body, 
  TPE_ClosestPointFunction env);

TPE_Vec3 TPE_bodyGetLinearVelocity(const TPE_Body *body);

/** Computes the minimum bounding box of given body. */
void TPE_bodyGetAABB(const TPE_Body *body, TPE_Vec3 *vMin, TPE_Vec3 *vMax);

/** Computes a bounding sphere of a body which is not minimal but faster to
  compute than the minimum bounding sphere. */
void TPE_bodyGetFastBSphere(const TPE_Body *body, TPE_Vec3 *center,
  TPE_Unit *radius);

/** Computes the minimum bounding sphere of a body (there is another function
  for a faster approximate bounding sphere). */
void TPE_bodyGetBSphere(const TPE_Body *body, TPE_Vec3 *center,
  TPE_Unit *radius);

unsigned char TPE_checkOverlapAABB(TPE_Vec3 v1Min, TPE_Vec3 v1Max, TPE_Vec3 v2Min,
  TPE_Vec3 v2Max);

/** Mostly for internal use, checks and potentiall resolves collision of two
  bodies so as to keep them outside given environment. Returns 1 if collision
  happened or 0 otherwise. */
unsigned char TPE_bodiesResolveCollision(TPE_Body *b1, TPE_Body *b2,
  TPE_ClosestPointFunction env);

/** Pins a joint of a body to specified location in space (sets its location
  and zeros its velocity). */
void TPE_jointPin(TPE_Joint *joint, TPE_Vec3 position);

/** "Fakes" a rotation of a moving sphere by rotating it in the direction of
  its movement; this can create the illusion of the sphere actually rotating
  due to friction even if the physics sphere object (a body with a single joint)
  isn't rotating at all. Returns a rotation in the "about axis" format (see
  library conventions). */
TPE_Vec3 TPE_fakeSphereRotation(TPE_Vec3 position1, TPE_Vec3 position2,
  TPE_Unit radius);

/** Casts a ray against environment and returns the closest hit of a surface. If
  no surface was hit, a vector with all elements equal to TPE_INFINITY will be
  returned. The function internally works differently for outside rays (rays
  cast from the outside of the environment) and inside rays. Outside rays can
  be traced with raymarching and will be processed very quickly and precisely;
  in this case if any intersection is found, the function will try to return a
  point outside (not guaranteed) the environment that's just in front of the hit
  surface. Inside rays are difficult and slow to trace because environment
  function won't provide distance, so the results aren't guaranteed to be
  precise (the ray may miss some intersections); here rays will be traced by
  given step (insideStepSize) and eventually iterated a bit towards the
  intersection -- if any intersection is found, the function will try to return
  a point inside (not guaranteed) the environment just before the hit
  surface. */
TPE_Vec3 TPE_castEnvironmentRay(TPE_Vec3 rayPos, TPE_Vec3 rayDir,
  TPE_ClosestPointFunction environment, TPE_Unit insideStepSize,
  TPE_Unit rayMarchMaxStep, unsigned int maxSteps);

/** Casts a ray against bodies in a world (ignoring the environment), returns
  the position of the closest hit as well as the hit body's index in bodyIndex
  (unless the bodyIndex pointer is 0 in which case it is ignored). Similarly
  with jointIndex. If no hit is found a vector with all elements equal to
  TPE_INFINITY will be returned and bodyIndex will be -1. A specific body can be
  excluded with excludeBody (negative value will just make this parameter
  ignored). */
TPE_Vec3 TPE_castBodyRay(TPE_Vec3 rayPos, TPE_Vec3 rayDir, short excludeBody,
  const TPE_World *world, short *bodyIndex, short *jointIndex);

/** Performs one step (tick, frame, ...) of the physics world simulation
  including updating positions and velocities of bodies, collision detection and
  resolution, possible reshaping or deactivation of inactive bodies etc. The
  time length of the step is relative to all other units but it's ideal if it is
  1/30th of a second. */
void TPE_worldStep(TPE_World *world);

void TPE_worldDeactivateAll(TPE_World *world);
void TPE_worldActivateAll(TPE_World *world);

TPE_Unit TPE_worldGetNetSpeed(const TPE_World *world);
TPE_Unit TPE_bodyGetNetSpeed(const TPE_Body *body);
TPE_Unit TPE_bodyGetAverageSpeed(const TPE_Body *body);
void TPE_bodyMultiplyNetSpeed(TPE_Body *body, TPE_Unit factor);
void TPE_bodyLimitAverageSpeed(TPE_Body *body, TPE_Unit speedMin,
  TPE_Unit speedMax);

/** Deactivates a body (puts it to sleep until another collision or force wake
  up). */
void TPE_bodyDeactivate(TPE_Body *body);

static unsigned char TPE_bodyIsActive(const TPE_Body *body);

/** Attempts to shift the joints of a soft body so that the tension of all
  springs becomes zero while keeping the joints near their current position.
  This function performs one iteration of the equalizing algorithm and doesn't
  guarantee a perfect solution, it may help to run multiple iterations (call
  this function multiple times). */
void TPE_bodyReshape(TPE_Body *body, TPE_ClosestPointFunction
  environmentFunction);

/** Mostly for internal use, performs some "magic" on body connections, mainly
  cancelling out of velocities going against each other and also applying
  connection friction in soft bodies. The strong parameter indicates if the
  body is soft or not. */
void TPE_bodyCancelOutVelocities(TPE_Body *body, unsigned char strong);

/** Moves a body by certain offset. */
void TPE_bodyMoveBy(TPE_Body *body, TPE_Vec3 offset);

/** Moves a body (its center of mass) to given position. */
void TPE_bodyMoveTo(TPE_Body *body, TPE_Vec3 position);

/** Zeros velocities of all soft body joints. */
void TPE_bodyStop(TPE_Body *body);

void TPE_bodyActivate(TPE_Body *body);

/** Adds velocity to a soft body. */
void TPE_bodyAccelerate(TPE_Body *body, TPE_Vec3 velocity);

void TPE_bodyApplyGravity(TPE_Body *body, TPE_Unit downwardsAccel);

/** Adds angular velocity to a soft body. The rotation vector specifies the axis
  of rotation by its direction and angular velocity by its magnitude (magnitude
  of TPE_FRACTIONS_PER_UNIT will add linear velocity of TPE_FRACTIONS_PER_UNIT
  per tick to a point in the distance of TPE_FRACTIONS_PER_UNIT from the
  rotation axis). */
void TPE_bodySpin(TPE_Body *body, TPE_Vec3 rotation);

/** Same as TPE_bodySpin but additionally allows to specify the center of
  the spin. */
void TPE_bodySpinWithCenter(TPE_Body *body, TPE_Vec3 rotation, TPE_Vec3 center);

/** Instantly rotates a body about an axis (see library conventions for
  the rotation format). */
void TPE_bodyRotateByAxis(TPE_Body *body, TPE_Vec3 rotation);

/** Computes the center of mass of a body. This averages the position of all
  joints; note that if you need, you may estimate the center of the body faster,
  e.g. by taking a position of a single "center joint", or averaging just 2
  extreme points. */ 
TPE_Vec3 TPE_bodyGetCenterOfMass(const TPE_Body *body);

/** Draws a debug view of a 3D physics world using a provided pixel drawing
  function. This can be used to overlay a simple visualization of the physics
  objects to your main render, to spot exact borders of objects etc. The
  function draws simple dotted lines and circles with different "colors" for
  different types of objects (joints, connections, environemnt). camPos, camRot
  and camView should match the camera settings of your main renderer. CamView.x
  is horizontal resolution in pixels, camView.y is the vertical resolution,
  CamView.z says the camera focal length (~FOV) in TPE_Units (0 means
  orthographic projection). envGridRes is the resolution of an environment probe
  grid (the function will probe points in space and draw borders of the physics
  environemnt), envGridSize is the size (int TPE_Units) of the grid cell. Note
  the function may be slow (reducing envGridRes can help, workable value can be
  e.g. 16). */
void TPE_worldDebugDraw(TPE_World *world, TPE_DebugDrawFunction drawFunc,
  TPE_Vec3 camPos, TPE_Vec3 camRot, TPE_Vec3 camView, unsigned short envGridRes,
  TPE_Unit envGridSize);

#define TPE_DEBUG_COLOR_CONNECTION 0
#define TPE_DEBUG_COLOR_JOINT 1
#define TPE_DEBUG_COLOR_ENVIRONMENT 2
#define TPE_DEBUG_COLOR_INACTIVE 3

unsigned int TPE_jointHash(const TPE_Joint *joint);
unsigned int TPE_connectionHash(const TPE_Connection *connection);
unsigned int TPE_bodyHash(const TPE_Body *body);

/** Computes 32 bit hash of the world, useful for checking if two states of the
  world differ. The function takes into account most of the relevant state but
  possibly not all of it, for details check the code. */
unsigned int TPE_worldHash(const TPE_World *world);

/* FUNCTIONS FOR GENERATING BODIES */

void TPE_makeBox(TPE_Joint joints[8], TPE_Connection connections[16],
  TPE_Unit width, TPE_Unit depth, TPE_Unit height, TPE_Unit jointSize);
void TPE_makeCenterBox(TPE_Joint joints[9], TPE_Connection connections[18],
  TPE_Unit width, TPE_Unit depth, TPE_Unit height, TPE_Unit jointSize);
void TPE_makeRect(TPE_Joint joints[4], TPE_Connection connections[6],
  TPE_Unit width, TPE_Unit depth, TPE_Unit jointSize);
void TPE_makeTriangle(TPE_Joint joints[3], TPE_Connection connections[3],
  TPE_Unit sideLength, TPE_Unit jointSize);
void TPE_makeCenterRect(TPE_Joint joints[5], TPE_Connection connections[8],
  TPE_Unit width, TPE_Unit depth, TPE_Unit jointSize);
void TPE_makeCenterRectFull(TPE_Joint joints[5], TPE_Connection connections[10],
  TPE_Unit width, TPE_Unit depth, TPE_Unit jointSize);
void TPE_make2Line(TPE_Joint joints[2], TPE_Connection connections[1],
  TPE_Unit length, TPE_Unit jointSize);

/* FUNCTIONS FOR BUILDING ENVIRONMENT */

TPE_Vec3 TPE_envAABoxInside(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 size);
TPE_Vec3 TPE_envAABox(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 maxCornerVec);
TPE_Vec3 TPE_envBox(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 maxCornerVec,
  TPE_Vec3 rotation);
TPE_Vec3 TPE_envSphere(TPE_Vec3 point, TPE_Vec3 center, TPE_Unit radius);
TPE_Vec3 TPE_envSphereInside(TPE_Vec3 point, TPE_Vec3 center, TPE_Unit radius);
TPE_Vec3 TPE_envHalfPlane(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 normal);
TPE_Vec3 TPE_envGround(TPE_Vec3 point, TPE_Unit height);
TPE_Vec3 TPE_envInfiniteCylinder(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3
  direction, TPE_Unit radius);
TPE_Vec3 TPE_envCylinder(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 direction,
  TPE_Unit radius);
TPE_Vec3 TPE_envCone(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 direction,
  TPE_Unit radius);
TPE_Vec3 TPE_envLineSegment(TPE_Vec3 point, TPE_Vec3 a, TPE_Vec3 b);
TPE_Vec3 TPE_envHeightmap(TPE_Vec3 point, TPE_Vec3 center, TPE_Unit gridSize,
  TPE_Unit (*heightFunction)(int x, int y), TPE_Unit maxDist);

/** Environment function for triagnular prism, e.g. for ramps. The sides array
  contains three 2D coordinates of points of the triangle in given plane with
  respect to the center. WARNING: the points must be specified in counter
  clowckwise direction! The direction var specified axis direction (0, 1 or
  2).*/
TPE_Vec3 TPE_envAATriPrism(TPE_Vec3 point, TPE_Vec3 center,
  const TPE_Unit sides[6], TPE_Unit depth, unsigned char direction);

/* The following are helper macros for creating a union of shapes inside an
  environment function and accelerating them with bounding volumes. */

#define TPE_ENV_START(test,point) TPE_Vec3 _pBest = test, _pTest; \
  TPE_Unit _dBest = TPE_DISTANCE(_pBest,point), _dTest; \
  (void)(_pBest); (void)(_dBest); (void)(_dTest); (void)(_pTest); /* supress war 
                                                                  */
#define TPE_ENV_NEXT(test,point) \
 { if (_pBest.x == point.x && _pBest.y == point.y && _pBest.z == point.z) \
     return _pBest; \
  _pTest = test; _dTest = TPE_DISTANCE(_pTest,point); \
  if (_dTest < _dBest) { _pBest = _pTest; _dBest = _dTest; } }

#define TPE_ENV_END return _pBest;

#define TPE_ENV_BCUBE_TEST(bodyBCubeC,bodyBCubeR,envBCubeC,envBCubeR) ( \
  (TPE_abs(envBCubeC.x - bodyBCubeC.x) <= ((bodyBCubeR) + (envBCubeR))) && \
  (TPE_abs(envBCubeC.y - bodyBCubeC.y) <= ((bodyBCubeR) + (envBCubeR))) && \
  (TPE_abs(envBCubeC.z - bodyBCubeC.z) <= ((bodyBCubeR) + (envBCubeR))))

#define TPE_ENV_BSPHERE_TEST(bodyBSphereC,bodyBSphereR,envBSphereC,envBSphereR)\
  (TPE_DISTANCE(bodyBSphereC,envBSphereC) <= ((bodyBSphereR) + (envBSphereR)))

//------------------------------------------------------------------------------
// privates:

unsigned short _TPE_body1Index, _TPE_body2Index, _TPE_joint1Index, _TPE_joint2Index;
TPE_CollisionCallback _TPE_collisionCallback;

static TPE_Unit TPE_nonZero(TPE_Unit x)
{
  return x != 0 ? x : 1;
}

static TPE_Unit TPE_connectionTension(TPE_Unit length,
  TPE_Unit desiredLength)
{
  return (length * TPE_F) / desiredLength
    - TPE_F;
}

TPE_Joint TPE_joint(TPE_Vec3 position, TPE_Unit size)
{
  TPE_Joint result;

  result.velocity[0] = 0;
  result.velocity[1] = 0;
  result.velocity[2] = 0;
 
  result.position = position;

  size /= TPE_JOINT_SIZE_MULTIPLIER;

  if (size > 0xff)
  {
    TPE_LOG("WARNING: joint size too big in TPE_joint");
  }

  result.sizeDivided = size;

  return result;
}

TPE_Vec3 TPE_vec3(TPE_Unit x, TPE_Unit y, TPE_Unit z)
{
  TPE_Vec3 r;

  r.x = x;
  r.y = y;
  r.z = z;

  return r;
}

TPE_Unit TPE_sqrt(TPE_Unit x)
{
  signed char sign = 1;
  unsigned int result = 0;
  unsigned int a = x;
  unsigned int b = 1u << 30;

  if (x < 0)
  {
    sign = -1;
    x *= -1;
  }

  while (b > a)
    b >>= 2;

  while (b != 0)
  {
    if (a >= result + b)
    {
      a -= result + b;
      result = result +  2 * b;
    }

    b >>= 2;
    result >>= 1;
  }

  return result * sign;
}

TPE_Unit TPE_vec3Len(TPE_Vec3 v)
{
#define ANTI_OVERFLOW 25000
  if  (v.x < ANTI_OVERFLOW && v.x > -1 * ANTI_OVERFLOW &&
       v.y < ANTI_OVERFLOW && v.y > -1 * ANTI_OVERFLOW &&
       v.z < ANTI_OVERFLOW && v.z > -1 * ANTI_OVERFLOW)
  {
    return  TPE_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
  }
  else
  {
    v.x /= 32; v.y /= 32; v.z /= 32;
    return  TPE_sqrt(v.x * v.x + v.y * v.y + v.z * v.z) * 32;
  }
#undef ANTI_OVERFLOW
}

TPE_Unit TPE_vec3LenApprox(TPE_Vec3 v)
{
  /* 48 sided polyhedron approximation */

  if (v.x < 0) v.x *= -1;
  if (v.y < 0) v.y *= -1;
  if (v.z < 0) v.z *= -1;
 
  if (v.x < v.y) /* order the coordinates */
  {
    if (v.x < v.z)
    {
      if (v.y < v.z)
      { /* v.x < v.y < v.z */
        int t = v.x; v.x = v.z; v.z = t;
      }
      else
      { /* v.x < v.z < v.y */
        int t = v.x; v.x = v.y; v.y = t;
        t = v.z; v.z = v.y; v.y = t;
      }
    }
    else
    { /* v.z < v.x < v.y */
      int t = v.x; v.x = v.y; v.y = t;
    }
  }
  else
  {
    if (v.y < v.z)
    {
      if (v.x < v.z)
      { /* v.y < v.x < v.z */
        int t = v.y; v.y = v.z; v.z = t;
        t = v.x; v.x = v.y; v.y = t;  
      }
      else
      { /* v.y < v.z < v.x */
        int t = v.y; v.y = v.z; v.z = t;
      }
    }
  }
    
  return (893 * v.x + 446 * v.y + 223 * v.z) / 1024;
}

TPE_Unit TPE_dist(TPE_Vec3 p1, TPE_Vec3 p2)
{
  p1 = TPE_vec3Minus(p1,p2);
  return TPE_vec3Len(p1); 
}

TPE_Unit TPE_distApprox(TPE_Vec3 p1, TPE_Vec3 p2)
{
  p1 = TPE_vec3Minus(p1,p2);
  return TPE_vec3LenApprox(p1); 
}

void TPE_bodyInit(TPE_Body *body, 
                  TPE_Joint *joints, 
                  unsigned char jointCount, 
                  TPE_Connection *connections, 
                  unsigned char connectionCount,
                  TPE_Unit mass)
{
  unsigned int i;

  body->joints = joints;
  body->jointCount = jointCount;
  body->connections = connections;
  body->connectionCount = connectionCount;
  body->deactivateCount = 0;
  body->friction = TPE_F / 2;
  body->elasticity = TPE_F / 2;
  body->flags = 0;
  body->jointMass = TPE_nonZero(mass / jointCount);

  for (i = 0; i < connectionCount; ++i)
  {
    TPE_Unit d = TPE_DISTANCE(
      joints[connections[i].joint1].position,
      joints[connections[i].joint2].position);

    if (d > 0xffff)
    {
      TPE_LOG("WARNING: joint distance too long in TPE_bodyInit");
    }

    connections[i].length = d != 0 ? d : 1; /* prevent later division by zero */
  }
}

void TPE_worldInit(TPE_World *world, TPE_Body *bodies, unsigned short bodyCount,
  TPE_ClosestPointFunction environmentFunction)
{
  world->bodies = bodies;
  world->bodyCount = bodyCount;
  world->environmentFunction = environmentFunction;
  world->collisionCallback = 0;
}
  
#define C(n,a,b) connections[n].joint1 = a; connections[n].joint2 = b;

void TPE_make2Line(TPE_Joint joints[2], TPE_Connection connections[1], 
  TPE_Unit length, TPE_Unit jointSize)
{
  joints[0] = TPE_joint(TPE_vec3(length / 2,0,0),jointSize);
  joints[1] = TPE_joint(TPE_vec3(length / -2,0,0),jointSize);
  C(0, 0,1)
}

void TPE_makeRect(TPE_Joint joints[4], 
                  TPE_Connection connections[6],
                  TPE_Unit width, 
                  TPE_Unit depth,  
                  TPE_Unit jointSize)
{
  unsigned char i;

  width /= 2;
  depth /= 2;

  for (i = 0; i < 4; ++i)
    joints[i] = TPE_joint(TPE_vec3((i % 2) ? -1 * width : width,
      0,(i / 2) ? - 1 * depth : depth),jointSize);

  C(0, 0,1) C(1, 0,2) C (2, 3,1) C(3, 3,2)
  C(4, 0,3) C(5, 1,2)
}

void TPE_makeCenterRect(TPE_Joint joints[5], 
                        TPE_Connection connections[8],
                        TPE_Unit width, 
                        TPE_Unit depth, 
                        TPE_Unit jointSize)
{
  TPE_makeRect(joints,connections,width,depth,jointSize);

  joints[4] = TPE_joint(TPE_vec3(0,0,0),jointSize);

  C(6, 0,4) C(7, 3,4)
}

void TPE_makeCenterRectFull(TPE_Joint joints[5], 
                            TPE_Connection connections[10],
                            TPE_Unit width, 
                            TPE_Unit depth, 
                            TPE_Unit jointSize)
{
  TPE_makeCenterRect(joints,connections,width,depth,jointSize);
  C(8, 1,4) C(9, 2,4)
}

void TPE_makeTriangle(TPE_Joint joints[3], 
                      TPE_Connection connections[3],
                      TPE_Unit sideLength, 
                      TPE_Unit jointSize)
{
  joints[0] = TPE_joint(TPE_vec3(sideLength / 2,0,
    TPE_sqrt((sideLength * sideLength) / 2) / 2),jointSize);

  joints[1] = joints[0];
  joints[1].position.x *= -1;
  joints[2] = TPE_joint(TPE_vec3(0,0,-1 * joints[0].position.z),jointSize);

  C(0, 0,1) C(1, 1,2) C(2, 2,0)
}

void TPE_makeBox(TPE_Joint joints[8], 
                 TPE_Connection connections[16],
                 TPE_Unit width, 
                 TPE_Unit depth, 
                 TPE_Unit height, 
                 TPE_Unit jointSize)
{
  unsigned char i;

  width /= 2;
  depth /= 2;
  height /= 2;

  for (i = 0; i < 8; ++i)
    joints[i] = TPE_joint( 
      TPE_vec3(  
        (i % 2) ? width : (-1 * width),
        ((i >> 2) % 2) ? height : (-1 * height),
        ((i >> 1) % 2) ? depth : (-1 * depth)),
      jointSize);

  C(0, 0,1) C(1, 1,3) C(2, 3,2) C(3, 2,0)  /* top */
  C(4, 4,5) C(5, 5,7) C(6, 7,6) C(7, 6,4)  /* bottom */
  C(8, 0,4) C(9, 1,5) C(10,3,7) C(11,2,6)  /* middle */
  C(12,0,7) C(13,1,6) C(14,2,5) C(15,3,4)  /* diagonal */
}

void TPE_makeCenterBox(TPE_Joint joints[9], TPE_Connection connections[18],
  TPE_Unit width, TPE_Unit depth, TPE_Unit height, TPE_Unit jointSize)
{
  TPE_makeBox(joints,connections,width,depth,height,jointSize);

  joints[8] = TPE_joint(TPE_vec3(0,0,0),jointSize);

  C(16, 0,8) C(17, 7,8)
}
  
#undef C

void TPE_bodyDeactivate(TPE_Body *body)
{
  body->flags |= TPE_BODY_FLAG_DEACTIVATED;
}

void TPE_worldStep(TPE_World *world)
{
  unsigned short i;
  unsigned short j;
  unsigned char k;
  TPE_Joint *joint;
  TPE_Joint *joint2;
  TPE_Vec3 origPos;
  TPE_Connection *connection;
  TPE_Vec3 aabbMin, aabbMax;
  unsigned char collided;
  TPE_Vec3 dir;
  TPE_Unit tension;

  _TPE_collisionCallback = world->collisionCallback;

  for (i = 0; i < world->bodyCount; ++i)
  {
    TPE_Body *body = world->bodies + i;   

    if (body->flags & (TPE_BODY_FLAG_DEACTIVATED | TPE_BODY_FLAG_DISABLED))
      continue; 

    /*TPE_Joint *joint = body->joints, *joint2;
    */

    joint = body->joints;

    origPos = body->joints[0].position;

    for (j = 0; j < body->jointCount; ++j) /* apply velocities */
    {
      /* non-rotating bodies will copy the 1st joint's velocity */

      if (body->flags & TPE_BODY_FLAG_NONROTATING)
        for (k = 0; k < 3; ++k)
          joint->velocity[k] = body->joints[0].velocity[k];

      joint->position.x += joint->velocity[0];
      joint->position.y += joint->velocity[1];
      joint->position.z += joint->velocity[2];

      joint++;
    }

    connection = body->connections;


    TPE_bodyGetAABB(body,&aabbMin,&aabbMax);
        
    _TPE_body1Index = i;
  
    _TPE_body2Index = _TPE_body1Index;

    collided = TPE_bodyEnvironmentResolveCollision(body,world->environmentFunction);

    if (body->flags & TPE_BODY_FLAG_NONROTATING)
    {
      /* Non-rotating bodies may end up still colliding after environment coll 
      resolvement (unlike rotating bodies where each joint is ensured separately
      to not collide). So if still in collision, we try a few more times. If not
      successful, we simply undo any shifts we've done. This should absolutely
      prevent any body escaping out of environment bounds. */
 
      for (i = 0; i < TPE_NONROTATING_COLLISION_RESOLVE_ATTEMPTS; ++i) 
      {
        if (!collided)
          break;

        collided =
          TPE_bodyEnvironmentResolveCollision(body,world->environmentFunction);
      }

      if (collided &&
        TPE_bodyEnvironmentCollide(body,world->environmentFunction))
        TPE_bodyMoveBy(body,TPE_vec3Minus(origPos,body->joints[0].position));
    }
    else /* normal, rotating bodies */
    {
      TPE_Unit bodyTension = 0;

      for (j = 0; j < body->connectionCount; ++j) /* joint tension */
      {
        joint  = &(body->joints[connection->joint1]);
        joint2 = &(body->joints[connection->joint2]);

        dir = TPE_vec3Minus(joint2->position,joint->position);

        tension = TPE_connectionTension(TPE_LENGTH(dir),
          connection->length);

        bodyTension += tension > 0 ? tension : -tension;

        if (tension > TPE_TENSION_ACCELERATION_THRESHOLD || 
          tension < -1 * TPE_TENSION_ACCELERATION_THRESHOLD)
        {
          TPE_vec3Normalize(&dir);

          if (tension > TPE_TENSION_GREATER_ACCELERATION_THRESHOLD ||
            tension < -1 * TPE_TENSION_GREATER_ACCELERATION_THRESHOLD)
          { 
            /* apply twice the acceleration after a second threshold, not so
               elegant but seems to work :) */
            dir.x *= 2;
            dir.y *= 2;
            dir.z *= 2;
          }

          dir.x /= TPE_TENSION_ACCELERATION_DIVIDER;
          dir.y /= TPE_TENSION_ACCELERATION_DIVIDER;
          dir.z /= TPE_TENSION_ACCELERATION_DIVIDER;

          if (tension < 0)
          {
            dir.x *= -1;
            dir.y *= -1;
            dir.z *= -1;
          }

          joint->velocity[0] += dir.x;
          joint->velocity[1] += dir.y;
          joint->velocity[2] += dir.z;

          joint2->velocity[0] -= dir.x;
          joint2->velocity[1] -= dir.y;
          joint2->velocity[2] -= dir.z;
        }

        connection++;
      }

      if (body->connectionCount > 0)
      {
        unsigned char hard = !(body->flags & TPE_BODY_FLAG_SOFT);

        if (hard)
        {
          TPE_bodyReshape(body,world->environmentFunction);

          bodyTension /= body->connectionCount;
        
          if (bodyTension > TPE_RESHAPE_TENSION_LIMIT)
            for (k = 0; k < TPE_RESHAPE_ITERATIONS; ++k)
              TPE_bodyReshape(body,world->environmentFunction);
        }
        
        if (!(body->flags & TPE_BODY_FLAG_SIMPLE_CONN))  
          TPE_bodyCancelOutVelocities(body,hard);
      }
    }

    for (j = 0; j < world->bodyCount; ++j)
    {
      if (j > i || (world->bodies[j].flags & TPE_BODY_FLAG_DEACTIVATED))
      {
        /* firstly quick-check collision of body AA bounding boxes */

        TPE_Vec3 aabbMin2, aabbMax2;
        TPE_bodyGetAABB(&world->bodies[j],&aabbMin2,&aabbMax2);

        _TPE_body2Index = j;

        if (TPE_checkOverlapAABB(aabbMin,aabbMax,aabbMin2,aabbMax2) &&
          TPE_bodiesResolveCollision(body,world->bodies + j,
          world->environmentFunction))
        {
          TPE_bodyActivate(body);
          body->deactivateCount = TPE_LIGHT_DEACTIVATION; 

          TPE_bodyActivate(world->bodies + j);
          world->bodies[j].deactivateCount = TPE_LIGHT_DEACTIVATION;
        }
      }
    }

    if (!(body->flags & TPE_BODY_FLAG_ALWAYS_ACTIVE))
    {
      if (body->deactivateCount >= TPE_DEACTIVATE_AFTER)
      {
        TPE_bodyStop(body);
        body->deactivateCount = 0;
        body->flags |= TPE_BODY_FLAG_DEACTIVATED;
      }
      else if (TPE_bodyGetAverageSpeed(body) <= TPE_LOW_SPEED)
        body->deactivateCount++;
      else
        body->deactivateCount = 0;
    }
  }
}

void TPE_bodyActivate(TPE_Body *body)
{
  /* the if check has to be here, don't remove it
  */

  if (body->flags & TPE_BODY_FLAG_DEACTIVATED)
  {
    TPE_bodyStop(body);
    body->flags &= ~TPE_BODY_FLAG_DEACTIVATED;
    body->deactivateCount = 0;
  }
}

TPE_Unit TPE_bodyGetNetSpeed(const TPE_Body *body)
{
  unsigned short i;

#if TPE_APPROXIMATE_NET_SPEED
  TPE_Vec3 netV = TPE_vec3(0,0,0);
    
  const TPE_Joint *joint = body->joints;

  for (i = 0; i < body->jointCount; ++i)
  {
    netV.x += TPE_abs(joint->velocity[0]);
    netV.y += TPE_abs(joint->velocity[1]);
    netV.z += TPE_abs(joint->velocity[2]);

    joint++;
  }

  return TPE_vec3LenApprox(netV);
#else
  TPE_Unit velocity = 0;

  const TPE_Joint *joint = body->joints;

  for (uint16_t i = 0; i < body->jointCount; ++i)
  {
    velocity += TPE_LENGTH(
      TPE_vec3(joint->velocity[0],joint->velocity[1],joint->velocity[2]));

    joint++;
  }

  return velocity;
#endif
}

TPE_Unit TPE_bodyGetAverageSpeed(const TPE_Body *body)
{
  return TPE_bodyGetNetSpeed(body) / body->jointCount;
}

void TPE_bodyMultiplyNetSpeed(TPE_Body *body, TPE_Unit factor)
{
  unsigned short j;
  unsigned char k;
  TPE_Joint *joint = body->joints;

  for (j = 0; j < body->jointCount; ++j)
  {
    for (k = 0; k < 3; ++k)
      joint->velocity[k] = 
        (((TPE_Unit) joint->velocity[k]) * factor) /
        TPE_F;

    joint++;
  }
}

void TPE_bodyLimitAverageSpeed(TPE_Body *body, 
                               TPE_Unit speedMin,
                               TPE_Unit speedMax)
{
  unsigned char i;
  TPE_Unit fraction;

  for (i = 0; i < 16; ++i)
  {
    TPE_Unit speed = TPE_bodyGetAverageSpeed(body);

    if (speed >= speedMin && speed <= speedMax)
      return;

    fraction =
      (((speedMax + speedMin) / 2) * TPE_F) /
      TPE_nonZero(speed);
    
    TPE_bodyMultiplyNetSpeed(body,fraction);
  }
}

void TPE_bodyCancelOutVelocities(TPE_Body *body, 
                                 unsigned char strong)
{
  unsigned short i;
  TPE_Vec3 avg;

  for (i = 0; i < body->connectionCount; ++i)
  {
    TPE_Connection *c = &body->connections[i];

    TPE_Joint *j1 = &(body->joints[c->joint1]);
    TPE_Joint *j2 = &(body->joints[c->joint2]);
    
    TPE_Vec3 dir = TPE_vec3Minus(j2->position,j1->position);

    TPE_Unit len = TPE_nonZero(TPE_LENGTH(dir));

    unsigned char cancel = 1;

    if (strong)
    {
      TPE_Unit tension = TPE_connectionTension(len,c->length);

      cancel = tension <= TPE_TENSION_ACCELERATION_THRESHOLD &&
        tension >= -1 * TPE_TENSION_ACCELERATION_THRESHOLD;
    }

    if (cancel)
    {
      TPE_Vec3
        v1 = TPE_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]),
        v2 = TPE_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]);

      dir.x = (dir.x * TPE_F) / len; /* normalize */
      dir.y = (dir.y * TPE_F) / len;
      dir.z = (dir.z * TPE_F) / len;

      v1 = TPE_vec3ProjectNormalized(v1,dir);
      v2 = TPE_vec3ProjectNormalized(v2,dir);

      avg = TPE_vec3Plus(v1,v2);

      avg.x /= 2;
      avg.y /= 2;
      avg.z /= 2;

      if (strong)
      {
        j1->velocity[0] = j1->velocity[0] - v1.x + avg.x;
        j1->velocity[1] = j1->velocity[1] - v1.y + avg.y;
        j1->velocity[2] = j1->velocity[2] - v1.z + avg.z;

        j2->velocity[0] = j2->velocity[0] - v2.x + avg.x;
        j2->velocity[1] = j2->velocity[1] - v2.y + avg.y;
        j2->velocity[2] = j2->velocity[2] - v2.z + avg.z;
      }
      else
      {
        j1->velocity[0] = j1->velocity[0] - v1.x + (v1.x * 3 + avg.x) / 4;
        j1->velocity[1] = j1->velocity[1] - v1.y + (v1.y * 3 + avg.y) / 4;
        j1->velocity[2] = j1->velocity[2] - v1.z + (v1.z * 3 + avg.z) / 4;

        j2->velocity[0] = j2->velocity[0] - v2.x + (v2.x * 3 + avg.x) / 4;
        j2->velocity[1] = j2->velocity[1] - v2.y + (v2.y * 3 + avg.y) / 4;
        j2->velocity[2] = j2->velocity[2] - v2.z + (v2.z * 3 + avg.z) / 4;
      }
    }
  }
}

void TPE_bodyReshape(TPE_Body *body, 
                     TPE_ClosestPointFunction environmentFunction)
{
  unsigned short i;
  TPE_Vec3 positionBackup;

  for (i = 0; i < body->connectionCount; ++i)
  {
    TPE_Connection *c = &body->connections[i];

    TPE_Joint *j1 = &(body->joints[c->joint1]);
    TPE_Joint *j2 = &(body->joints[c->joint2]);
      
    TPE_Vec3 dir = TPE_vec3Minus(j2->position,j1->position);

    TPE_Vec3 middle = TPE_vec3Plus(j1->position,j2->position);

    middle.x /= 2;
    middle.y /= 2;
    middle.z /= 2;

    TPE_vec3Normalize(&dir);

    dir.x = (dir.x * c->length) / TPE_F;
    dir.y = (dir.y * c->length) / TPE_F;
    dir.z = (dir.z * c->length) / TPE_F;

    positionBackup = j1->position;

    j1->position.x = middle.x - dir.x / 2;
    j1->position.y = middle.y - dir.y / 2;
    j1->position.z = middle.z - dir.z / 2;

    if (environmentFunction != 0 && TPE_LENGTH(TPE_vec3Minus(j1->position,
      environmentFunction(j1->position,TPE_JOINT_SIZE(*j1))))
      < TPE_JOINT_SIZE(*j1))
      j1->position = positionBackup;
  
    positionBackup = j2->position;

    j2->position.x = j1->position.x + dir.x;
    j2->position.y = j1->position.y + dir.y;
    j2->position.z = j1->position.z + dir.z; 

    if (environmentFunction != 0 && TPE_LENGTH(TPE_vec3Minus(j2->position,
      environmentFunction(j2->position,TPE_JOINT_SIZE(*j2))))
      < TPE_JOINT_SIZE(*j2))
      j2->position = positionBackup;
  }
}

TPE_Vec3 TPE_vec3Plus(TPE_Vec3 v1, TPE_Vec3 v2)
{
  v1.x += v2.x;
  v1.y += v2.y;
  v1.z += v2.z;

  return v1;
}

TPE_Vec3 TPE_vec3Minus(TPE_Vec3 v1, TPE_Vec3 v2)
{
  v1.x -= v2.x;
  v1.y -= v2.y;
  v1.z -= v2.z;

  return v1;
}

void TPE_vec3Normalize(TPE_Vec3 *v)
{
  TPE_Unit l = TPE_LENGTH(*v);

  if (l == 0)
    *v = TPE_vec3(TPE_F,0,0);
  else
  {
    if (l < 16) /* too short vec would cause inacurracte normalization */
    {
      v->x *= 8;
      v->y *= 8;
      v->z *= 8;
      l = TPE_LENGTH(*v);
    }

    v->x = (v->x * TPE_F) / l;
    v->y = (v->y * TPE_F) / l;
    v->z = (v->z * TPE_F) / l;
  }
}

TPE_Vec3 TPE_bodyGetRotation(const TPE_Body *body, 
                             unsigned short joint1, 
                             unsigned short joint2, 
                             unsigned short joint3)
{
  return TPE_rotationFromVecs(
    TPE_vec3Minus(
      body->joints[joint2].position,
      body->joints[joint1].position),
    TPE_vec3Minus(
      body->joints[joint3].position,
      body->joints[joint1].position));
}

TPE_Vec3 TPE_bodyGetCenterOfMass(const TPE_Body *body)
{
  /* note that joint sizes don't play a role as all weight the same */
  unsigned short i;
  TPE_Vec3 result = TPE_vec3(0,0,0);

  const TPE_Joint *j = body->joints;

  for (i = 0; i < body->jointCount; ++i)
  {
    result = TPE_vec3Plus(result,j->position);
    j++;
  }

  result.x /= body->jointCount;
  result.y /= body->jointCount;
  result.z /= body->jointCount;
 
  return result;
}

void TPE_bodySpinWithCenter(TPE_Body *body, 
                            TPE_Vec3 rotation, 
                            TPE_Vec3 center)
{
  unsigned short i;

  for (i = 0; i < body->jointCount; ++i)
  {
    TPE_Joint *j = body->joints + i;

    TPE_Vec3 toPoint = TPE_vec3Minus(j->position,center);

    toPoint = TPE_vec3Project(toPoint,rotation);
    toPoint = TPE_vec3Plus(center,toPoint);
    toPoint = TPE_vec3Minus(j->position,toPoint);
    toPoint = TPE_vec3Cross(toPoint,rotation);

    j->velocity[0] += toPoint.x;
    j->velocity[1] += toPoint.y;
    j->velocity[2] += toPoint.z;
  }
}

void TPE_bodySpin(TPE_Body *body, TPE_Vec3 rotation)
{
  TPE_bodySpinWithCenter(body,rotation,TPE_bodyGetCenterOfMass(body));
}

TPE_Vec3 _TPE_rotateByAxis(TPE_Vec3 p, 
                           TPE_Vec3 axisNormalized, 
                           TPE_Unit angle)
{
  TPE_Vec3 b;

  TPE_Vec3 projected = TPE_vec3ProjectNormalized(p,axisNormalized);

  TPE_Vec3 a = TPE_vec3Minus(p,projected);

  if (a.x == 0 && a.y == 0 && a.z == 0)
    return p;

  b = TPE_vec3Cross(a,axisNormalized);

  return TPE_vec3Plus(projected,TPE_vec3Plus(
    TPE_vec3Times(a,TPE_cos(angle)),
    TPE_vec3Times(b,TPE_sin(angle))));
}

void TPE_bodyRotateByAxis(TPE_Body *body, TPE_Vec3 rotation)
{
  unsigned short i;

  TPE_Vec3 bodyCenter = TPE_bodyGetCenterOfMass(body);
  TPE_Unit angle = TPE_LENGTH(rotation);

  TPE_vec3Normalize(&rotation);

  for (i = 0; i < body->jointCount; ++i)
  {
    TPE_Vec3 toPoint = TPE_vec3Minus(body->joints[i].position,bodyCenter);
    body->joints[i].position = TPE_vec3Plus(bodyCenter,
    _TPE_rotateByAxis(toPoint,rotation,angle));
  }
}

TPE_Vec3 TPE_vec3Cross(TPE_Vec3 v1, TPE_Vec3 v2)
{
  TPE_Vec3 r;

  r.x = (v1.y * v2.z - v1.z * v2.y) / TPE_F;
  r.y = (v1.z * v2.x - v1.x * v2.z) / TPE_F;
  r.z = (v1.x * v2.y - v1.y * v2.x) / TPE_F;

  return r;
}

TPE_Vec3 TPE_vec3ProjectNormalized(TPE_Vec3 v, TPE_Vec3 baseNormalized)
{
  TPE_Vec3 r;

  TPE_Unit p = TPE_vec3Dot(v,baseNormalized);

  r.x = (p * baseNormalized.x) / TPE_F;
  r.y = (p * baseNormalized.y) / TPE_F;
  r.z = (p * baseNormalized.z) / TPE_F;
  
  return r;
}

TPE_Vec3 TPE_vec3Project(TPE_Vec3 v, TPE_Vec3 base)
{
  TPE_vec3Normalize(&base);

  return TPE_vec3ProjectNormalized(v,base);
}

void TPE_bodyMoveBy(TPE_Body *body, 
                    TPE_Vec3 offset)
{
  unsigned short i;

  for (i = 0; i < body->jointCount; ++i)
    body->joints[i].position = TPE_vec3Plus(body->joints[i].position,
      offset);
}

void TPE_bodyApplyGravity(TPE_Body *body, 
                          TPE_Unit downwardsAccel)
{
  unsigned short i;

  if ((body->flags & TPE_BODY_FLAG_DEACTIVATED) ||
      (body->flags & TPE_BODY_FLAG_DISABLED))
    return;

  for (i = 0; i < body->jointCount; ++i)
    body->joints[i].velocity[1] -= downwardsAccel;
}

void TPE_bodyAccelerate(TPE_Body *body, 
                        TPE_Vec3 velocity)
{
  unsigned short i;
  TPE_bodyActivate(body);

  for (i = 0; i < body->jointCount; ++i)
  {
    body->joints[i].velocity[0] += velocity.x;
    body->joints[i].velocity[1] += velocity.y;
    body->joints[i].velocity[2] += velocity.z;
  }
}

void TPE_bodyStop(TPE_Body *body)
{
  unsigned short i;

  for (i = 0; i < body->jointCount; ++i)
  {
    body->joints[i].velocity[0] = 0;
    body->joints[i].velocity[1] = 0;
    body->joints[i].velocity[2] = 0;
  }
}

void _TPE_bodyNonrotatingJointCollided(TPE_Body *b, 
                                       short jointIndex, 
                                       TPE_Vec3 origPos, 
                                       unsigned char success)
{
  unsigned short i;
  unsigned char j;

  origPos = TPE_vec3Minus(b->joints[jointIndex].position,origPos);

  for (i = 0; i < b->jointCount; ++i)
    if (i != jointIndex)
    {
      b->joints[i].position = TPE_vec3Plus(b->joints[i].position,origPos);
     
      if (success) 
        for (j = 0; j < 3; ++j)
          b->joints[i].velocity[j] = b->joints[jointIndex].velocity[j];
    }
}

TPE_Unit TPE_vec3Dot(TPE_Vec3 v1, TPE_Vec3 v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / TPE_F;
}

TPE_Unit TPE_cos(TPE_Unit x)  
{
  return TPE_sin(x + TPE_F / 4);
}

TPE_Unit TPE_sin(TPE_Unit x)  
{
  signed char sign = 1;
  TPE_Unit tmp;
    
  if (x < 0) /* odd function */
  {
    x *= -1;
    sign = -1;
  }
    
  x %= TPE_F;
  
  if (x > TPE_F / 2)
  {
    x -= TPE_F / 2;
    sign *= -1;
  }

  tmp = TPE_F - 2 * x;
 
  #define _PI2 5053 /* 9.8696044 * TPE_F */
  return sign * /* Bhaskara's approximation */
    (((32 * x * _PI2) / TPE_F) * tmp) / 
    ((_PI2 * (5 * TPE_F - (8 * x * tmp) / 
      TPE_F)) / TPE_F);
  #undef _PI2
}

unsigned char TPE_bodiesResolveCollision(TPE_Body *b1, TPE_Body *b2,
  TPE_ClosestPointFunction env)
{
  unsigned char r = 0;
  unsigned short i;
  unsigned short j;

  for (i = 0; i < b1->jointCount; ++i)
    for (j = 0; j < b2->jointCount; ++j)
    {
      TPE_Vec3 origPos2 = b2->joints[j].position;
      TPE_Vec3 origPos1 = b1->joints[i].position;

      _TPE_joint1Index = i;
      _TPE_joint2Index = j;

      if (TPE_jointsResolveCollision(&(b1->joints[i]),&(b2->joints[j]),
        b1->jointMass,b2->jointMass,(b1->elasticity + b2->elasticity) / 2,
        (b1->friction + b2->friction) / 2,env))
      {
        r = 1;

        if (b1->flags & TPE_BODY_FLAG_NONROTATING)
          _TPE_bodyNonrotatingJointCollided(b1,i,origPos1,1);

        if (b2->flags & TPE_BODY_FLAG_NONROTATING)
          _TPE_bodyNonrotatingJointCollided(b2,j,origPos2,1);
      }
    }

  return r;
}

unsigned char TPE_jointsResolveCollision(TPE_Joint *j1, 
                                         TPE_Joint *j2,
                                         TPE_Unit mass1, 
                                         TPE_Unit mass2, 
                                         TPE_Unit elasticity, 
                                         TPE_Unit friction,
                                         TPE_ClosestPointFunction env)
{

  TPE_Vec3 pos1Backup, pos2Backup;
  TPE_Unit ratio;
  TPE_Unit shiftDistance;
  TPE_Vec3 shift;
  TPE_Unit v1, v2;
  TPE_Vec3 vel;
  TPE_Vec3 frictionVec;

  TPE_Vec3 dir = TPE_vec3Minus(j2->position,j1->position);

  TPE_Unit d = TPE_LENGTH(dir) - TPE_JOINT_SIZE(*j1) - TPE_JOINT_SIZE(*j2);

  if (d < 0) /* collision? */
  {
    if (_TPE_collisionCallback != 0 && !_TPE_collisionCallback(
        _TPE_body1Index,_TPE_joint1Index,_TPE_body2Index,_TPE_joint2Index,
        TPE_vec3Plus(j1->position,dir)))
      return 0;

      pos1Backup = j1->position;
      pos2Backup = j2->position;
  
    /* separate joints, the shift distance will depend on the weight ratio:
    */

    d = -1 * d + TPE_COLLISION_RESOLUTION_MARGIN;

    TPE_vec3Normalize(&dir);

    ratio = (mass2 * TPE_F) / 
      TPE_nonZero(mass1 + mass2);

    shiftDistance = (ratio * d) / TPE_F;

    shift = TPE_vec3Times(dir,shiftDistance);

    j1->position = TPE_vec3Minus(j1->position,shift);

    shiftDistance = d - shiftDistance;

    shift = TPE_vec3Times(dir,shiftDistance);

    j2->position = TPE_vec3Plus(j2->position,shift);

    /* compute new velocities:
    */

    vel = TPE_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]);

    vel = TPE_vec3Project(vel,dir);

    j1->velocity[0] = j1->velocity[0] - vel.x;
    j1->velocity[1] = j1->velocity[1] - vel.y;
    j1->velocity[2] = j1->velocity[2] - vel.z;

    /* friction explanation: Not physically correct (doesn't depend on load), 
    friction basically means we weighted average the velocities of the bodies
    in the direction perpendicular to the hit normal, in the ratio of their
    masses, friction coefficient just says how much of this effect we apply
    (it multiplies the friction vectors we are subtracting) */

    frictionVec = TPE_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]);

    v1 = TPE_vec3Dot(vel,dir);
    vel = TPE_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]);
    vel = TPE_vec3Project(vel,dir);

    j2->velocity[0] = j2->velocity[0] - vel.x;
    j2->velocity[1] = j2->velocity[1] - vel.y;
    j2->velocity[2] = j2->velocity[2] - vel.z;

    frictionVec = TPE_vec3Minus(
      TPE_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]),
      frictionVec);

    v2 = TPE_vec3Dot(vel,dir);

    TPE_getVelocitiesAfterCollision(&v1,&v2,mass1,mass2,elasticity);

    vel = TPE_vec3Times(dir,v1);

#define assignVec(j,i,d,o) \
  j->velocity[i] = j->velocity[i] + vel.d o (((frictionVec.d * ratio) / \
    TPE_F) * friction) / TPE_F;

    assignVec(j1,0,x,+)
    assignVec(j1,1,y,+)
    assignVec(j1,2,z,+)

    vel = TPE_vec3Times(dir,v2);

    ratio = TPE_F - ratio;

    assignVec(j2,0,x,-)
    assignVec(j2,1,y,-)
    assignVec(j2,2,z,-)

#undef assignVec

    if (env != 0)
    {
      /* ensure the joints aren't colliding with environment */

      if (TPE_jointEnvironmentResolveCollision(j1,elasticity,friction,env) == 2)
        j1->position = pos1Backup;

      if (TPE_jointEnvironmentResolveCollision(j2,elasticity,friction,env) == 2)
        j2->position = pos2Backup;
    }

    return 1;
  }

  return 0;
}

TPE_Vec3 TPE_vec3Times(TPE_Vec3 v, TPE_Unit units)
{
  v.x = (v.x * units) / TPE_F;
  v.y = (v.y * units) / TPE_F;
  v.z = (v.z * units) / TPE_F;

  return v;
}

TPE_Vec3 TPE_vec3TimesPlain(TPE_Vec3 v, TPE_Unit q)
{
  v.x *= q;
  v.y *= q;
  v.z *= q;

  return v;
}

void TPE_getVelocitiesAfterCollision(TPE_Unit *v1, TPE_Unit *v2,
  TPE_Unit m1, TPE_Unit m2, TPE_Unit elasticity)
{
  /* In the following a lot of TPE_F cancel out, feel free to
     check if confused. */

  TPE_Unit m1Pm2 = TPE_nonZero(m1 + m2);
  TPE_Unit v2Mv1 = TPE_nonZero(*v2 - *v1);

  TPE_Unit m1v1Pm2v2 = ((m1 * *v1) + (m2 * *v2));

  *v1 = (((elasticity * m2 / TPE_F) * v2Mv1)
    + m1v1Pm2v2) / m1Pm2;

  *v2 = (((elasticity * m1 / TPE_F) * -1 * v2Mv1)
    + m1v1Pm2v2) / m1Pm2;
}

unsigned char TPE_jointEnvironmentResolveCollision(TPE_Joint *joint,
                                                   TPE_Unit elasticity, 
                                                   TPE_Unit friction, 
                                                   TPE_ClosestPointFunction env)
{
  TPE_Vec3 positionBackup, shift;
  unsigned char success = 0;
  int i;
  TPE_Vec3 vel2;

  TPE_Vec3 toJoint =
    TPE_vec3Minus(joint->position,env(joint->position,TPE_JOINT_SIZE(*joint)));

  TPE_Unit len = TPE_LENGTH(toJoint);

  if (len <= TPE_JOINT_SIZE(*joint))
  {
    if (_TPE_collisionCallback != 0)
      if (!_TPE_collisionCallback(_TPE_body1Index,
        _TPE_joint1Index,_TPE_body2Index,_TPE_joint2Index,
        TPE_vec3Minus(joint->position,toJoint)))
        return 0;

    /* colliding */

    positionBackup = joint->position;

    if (len > 0)
    {
      /* Joint center is still outside the geometry so we can determine the
         normal and use it to shift it outside. This can still leave the joint
         colliding though, so try to repeat it a few times. */

      for (i = 0; i < TPE_COLLISION_RESOLUTION_ITERATIONS; ++i)
      {
        shift = toJoint;

        TPE_vec3Normalize(&shift); 

        shift = TPE_vec3Times(shift,TPE_JOINT_SIZE(*joint) - len + 
          TPE_COLLISION_RESOLUTION_MARGIN);
          
        joint->position = TPE_vec3Plus(joint->position,shift);
  
        toJoint = TPE_vec3Minus(joint->position,env(joint->position,
          TPE_JOINT_SIZE(*joint)));

        len = TPE_LENGTH(toJoint); /* still colliding? */

        if (len >= TPE_JOINT_SIZE(*joint))
        {
          success = 1;
          break;
        }
      }
    }

    if (!success)
    {
      /* Shifting along normal was unsuccessfull, now try different approach:
         shift back by joint velocity. */

      shift = TPE_vec3(-1 * joint->velocity[0],-1 * joint->velocity[1],
        -1 * joint->velocity[2]);
      
      for (i = 0; i < TPE_COLLISION_RESOLUTION_ITERATIONS; ++i)
      {
        joint->position = TPE_vec3Plus(joint->position,shift);

        toJoint = TPE_vec3Minus(joint->position,
          env(joint->position,TPE_JOINT_SIZE(*joint)));

        len = TPE_LENGTH(toJoint); /* still colliding? */

        if (len >= TPE_JOINT_SIZE(*joint))
        {
          success = 1;
          break;
        }

        shift.x /= 2; /* decrease the step a bit */
        shift.y /= 2;
        shift.z /= 2;
      }
    }

    if (success)
    {
      TPE_Vec3 vel = TPE_vec3(joint->velocity[0],joint->velocity[1],
        joint->velocity[2]);

      vel = TPE_vec3Project(vel,shift); /* parallel part of velocity */

      vel2 = TPE_vec3Minus( /* perpendicular part of velocity */
        TPE_vec3(joint->velocity[0],joint->velocity[1],joint->velocity[2]),vel);

      vel2 = TPE_vec3Times(vel2,friction);

      vel = TPE_vec3Times(vel,TPE_F + elasticity);

      joint->velocity[0] -= vel.x + vel2.x;
      joint->velocity[1] -= vel.y + vel2.y;
      joint->velocity[2] -= vel.z + vel2.z;
    }
    else
    {
      TPE_LOG("WARNING: joint-environment collision couldn't be resolved");

      joint->position = positionBackup;
      joint->velocity[0] = 0;
      joint->velocity[1] = 0;
      joint->velocity[2] = 0;

      return 2;
    }

    return 1;
  }

  return 0;
}

unsigned char TPE_bodyEnvironmentCollide(const TPE_Body *body,
                                         TPE_ClosestPointFunction env)
{
  unsigned short i;

  for (i = 0; i < body->jointCount; ++i)
  {
    const TPE_Joint *joint = body->joints + i;

    TPE_Unit size = TPE_JOINT_SIZE(*joint);

    if (TPE_DISTANCE(joint->position,env(joint->position,size)) <= size)
      return 1;
  }

  return 0;
}

void TPE_bodyGetFastBSphere(const TPE_Body *body, TPE_Vec3 *center,
  TPE_Unit *radius)
{
  TPE_Vec3 b;

  TPE_bodyGetAABB(body,center,&b);

  center->x = (center->x + b.x) / 2;
  center->y = (center->y + b.y) / 2;
  center->z = (center->z + b.z) / 2;

  *radius = TPE_DISTANCE(*center,b);
}

void TPE_bodyGetBSphere(const TPE_Body *body, TPE_Vec3 *center,
  TPE_Unit *radius)
{
  const TPE_Joint *j;
  unsigned short i;
  TPE_Unit distSquared;

  *radius = TPE_INFINITY;
  *center = TPE_bodyGetCenterOfMass(body);

  j = body->joints;

  for (i = 0; i < body->jointCount; ++i)
  {
    TPE_Vec3 diff;

    TPE_Unit js = TPE_JOINT_SIZE(*j);

    /* Sadly we have to have these conditions here which slow this down. If we
       were only computing a BB sphere of a point cloud, we wouldn't have to
       compute abs vals (as squaring would effectively compute them), but here
       we need to add joint size which needs to know about the sign. */

    diff.x = ((center->x > j->position.x) ?
      (center->x - j->position.x) : (j->position.x - center->x)) + js;

    diff.y = ((center->y > j->position.y) ?
      (center->y - j->position.y) : (j->position.y - center->y)) + js;

    diff.z = ((center->z > j->position.z) ?
      (center->z - j->position.z) : (j->position.z - center->z)) + js;

    distSquared = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

    if (distSquared < *radius)
      *radius = distSquared;

    j++;
  }

  *radius = TPE_sqrt(*radius);
}

unsigned char TPE_bodyEnvironmentResolveCollision(TPE_Body *body, 
                                                  TPE_ClosestPointFunction env)
{
  TPE_Vec3 c;
  TPE_Unit d;
  unsigned short i;
  unsigned char collision = 0;
  unsigned char r;

  TPE_bodyGetFastBSphere(body,&c,&d);

  if (TPE_DISTANCE(c,env(c,d)) > d)
    return 0;

  /* now test the full body collision:
  */

  for (i = 0; i < body->jointCount; ++i)
  {
    TPE_Vec3 previousPos = body->joints[i].position;

    _TPE_joint1Index = i;

    r = TPE_jointEnvironmentResolveCollision(
      body->joints + i,body->elasticity,body->friction,env);

    if (r)
    {
      collision = 1;

      if (body->flags & TPE_BODY_FLAG_NONROTATING)
        _TPE_bodyNonrotatingJointCollided(body,i,previousPos,r == 1);
    }
  }

  return collision;
}

TPE_Vec3 TPE_vec3Normalized(TPE_Vec3 v)
{
  TPE_vec3Normalize(&v);
  return v;
}

TPE_Unit TPE_atan(TPE_Unit x)
{
  /* atan approximation by polynomial 
     WARNING: this will break with different value of TPE_FRACTIONS_PER_UNIT 
  */

  TPE_Unit sign = 1, x2 = x * x;

  if (x < 0)
  {
    x *= -1;
    sign = -1;
  }

  if (x > 30000) /* anti overflow */
    return sign * (TPE_F / 4);

  return sign *
    (307 * x + x2) / ((267026 + 633 * x + x2) / 128);
}

void _TPE_vec2Rotate(TPE_Unit *x, TPE_Unit *y, TPE_Unit angle)
{
  TPE_Unit tmp = *x;

  TPE_Unit s = TPE_sin(angle);
  TPE_Unit c = TPE_cos(angle);

  *x = (c * *x - s * *y) / TPE_F;
  *y = (s * tmp + c * *y) / TPE_F;
}

TPE_Unit TPE_vec2Angle(TPE_Unit x, TPE_Unit y)
{
  TPE_Unit r = 0;

  if (x != 0)
  {
    r = TPE_atan((y * TPE_F) / x);

    if (x < 0)
      r += TPE_F / 2;
    else if (r < 0)
      r += TPE_F;
  }
  else
  {
    if (y < 0)
      r = (3 * TPE_F) / 4;
    else if (y > 0)
      r = TPE_F / 4;
    /* else (y == 0) r stays 0 */
  }

  return r;
}

TPE_Vec3 TPE_rotationFromVecs(TPE_Vec3 forward, TPE_Vec3 right)
{
  TPE_Vec3 result;

  /* get rotation around Y:
  */

  result.y = TPE_vec2Angle(forward.z,-1 * forward.x);

  /* now rotate back by this angle to align with x = 0 plane:
  */


  _TPE_vec2Rotate(&forward.z,&forward.x,result.y);
  _TPE_vec2Rotate(&right.z,&right.x,result.y);

  /* now do the same for the second axis:
  */

  result.x = 
    TPE_vec2Angle(forward.z,forward.y);

  _TPE_vec2Rotate(&right.z,&right.y,-1 * result.x);

  result.z = TPE_vec2Angle(right.x,-1 * right.y);

  return result;
}

TPE_Vec3 _TPE_project3DPoint(TPE_Vec3 p, 
                             TPE_Vec3 camPos, 
                             TPE_Vec3 camRot,
                             TPE_Vec3 camView)
{
  /* transform to camera space:
  */

  p = TPE_vec3Minus(p,camPos);

  _TPE_vec2Rotate(&p.z,&p.x,camRot.y);
  _TPE_vec2Rotate(&p.z,&p.y,-1 * camRot.x);
  _TPE_vec2Rotate(&p.y,&p.x,-1 * camRot.z);

  if (p.z <= 0)
    return p;

  if (camView.z != 0)
  {
    /* perspective */

    p.x = (p.x * camView.z) / p.z;
    p.y = (p.y * camView.z) / p.z;

    p.x = camView.x / 2 + (p.x * camView.x) / (2 * TPE_F);
    p.y = camView.y / 2 - (p.y * camView.x) / (2 * TPE_F);
                                      /* ^ x here intentional */
  }
  else
  {
    /* ortho */

    p.x = camView.x / 2 + p.x;
    p.y = camView.y / 2 - p.y;
  }

  return p;
}

void _TPE_drawDebugPixel(
  TPE_Unit x, TPE_Unit y, TPE_Unit w, TPE_Unit h, unsigned char c,
  TPE_DebugDrawFunction f)
{
  if (x >= 0 && x < w && y >= 0 && y < h)
    f(x,y,c);
}

void TPE_worldDebugDraw(TPE_World *world, 
                        TPE_DebugDrawFunction drawFunc,
                        TPE_Vec3 camPos, 
                        TPE_Vec3 camRot, 
                        TPE_Vec3 camView, 
                        unsigned short envGridRes,
                        TPE_Unit envGridSize)
{
#define Z_LIMIT 250
  unsigned char i;
  unsigned char j;
  unsigned char k;
  unsigned char l;
  unsigned char c;
  TPE_Vec3 diff;
  TPE_Unit size;


  if (world->environmentFunction != 0)
  {
    // environment:

    TPE_Vec3 testPoint;

    TPE_Unit gridHalfSize = (envGridSize * envGridRes) / 2;

    TPE_Vec3 center;

    if (envGridRes != 0)
    {
      center = TPE_vec3(0,TPE_sin(camRot.x),TPE_cos(camRot.x));

      _TPE_vec2Rotate(&center.x,&center.z,camRot.y);

      center = TPE_vec3Times(center,gridHalfSize);
      center = TPE_vec3Plus(camPos,center);

      center.x = (center.x / envGridSize) * envGridSize;
      center.y = (center.y / envGridSize) * envGridSize;
      center.z = (center.z / envGridSize) * envGridSize;
    }

    testPoint.y = center.y - gridHalfSize;

    for (j = 0; j < envGridRes; ++j)
    {
      testPoint.x = center.x - gridHalfSize;

      for (k = 0; k < envGridRes; ++k)
      {
        testPoint.z = center.z - gridHalfSize;

        for (l = 0; l < envGridRes; ++l)
        {
          TPE_Vec3 r = world->environmentFunction(testPoint,envGridSize);

          if (r.x != testPoint.x || r.y != testPoint.y || r.z != testPoint.z)
          {
            r = _TPE_project3DPoint(r,camPos,camRot,camView);
 
            if (r.z > Z_LIMIT)
              _TPE_drawDebugPixel(r.x,r.y,camView.x,camView.y,
                TPE_DEBUG_COLOR_ENVIRONMENT,drawFunc);
          }

          testPoint.z += envGridSize;
        }

        testPoint.x += envGridSize;
      }

      testPoint.y += envGridSize;
    }
  }

  for (i = 0; i < world->bodyCount; ++i)
  {
    /*connections:
    */
    for (j = 0; j < world->bodies[i].connectionCount; ++j)
    {
      TPE_Vec3
        p1 = world->bodies[i].joints[
          world->bodies[i].connections[j].joint1].position,
        p2 = world->bodies[i].joints[
          world->bodies[i].connections[j].joint2].position;

      p1 = _TPE_project3DPoint(p1,camPos,camRot,camView);
      p2 = _TPE_project3DPoint(p2,camPos,camRot,camView);

      if (p1.z <= Z_LIMIT || p2.z <= Z_LIMIT)
        continue;

      diff = TPE_vec3Minus(p2,p1);

#define SEGS 16

      c = (world->bodies[i].flags & TPE_BODY_FLAG_DEACTIVATED) ?
        TPE_DEBUG_COLOR_INACTIVE : TPE_DEBUG_COLOR_CONNECTION;

      for (k = 0; k < SEGS; ++k)
      {
        p2.x = p1.x + (diff.x * k) / SEGS;
        p2.y = p1.y + (diff.y * k) / SEGS;

        _TPE_drawDebugPixel(p2.x,p2.y,camView.x,camView.y,c,drawFunc);
      }
#undef SEGS
    }

    /* joints:
    */
    for (j = 0; j < world->bodies[i].jointCount; ++j)
    {
      TPE_Vec3 p = _TPE_project3DPoint(world->bodies[i].joints[j].position,
        camPos,camRot,camView);

      if (p.z > Z_LIMIT)
      {
        unsigned char color = (world->bodies[i].flags & TPE_BODY_FLAG_DEACTIVATED) ?
          TPE_DEBUG_COLOR_INACTIVE : TPE_DEBUG_COLOR_JOINT;

        _TPE_drawDebugPixel(p.x,p.y,camView.x,camView.y,color,drawFunc);

        size = TPE_JOINT_SIZE(world->bodies[i].joints[j]);

        if (camView.z != 0) /* not ortho? */
        {
          size /= 2;
          size = (size * camView.x) / TPE_F;
          size = (size * camView.z) / p.z;
        }

#define SEGS 4
        for (k = 0; k < SEGS + 1; ++k)
        {
          TPE_Unit 
            dx = (TPE_sin(TPE_F * k / (8 * SEGS)) * size)
              / TPE_F,
            dy = (TPE_cos(TPE_F * k / (8 * SEGS)) * size)
              / TPE_F;

#define dp(a,b,c,d) \
  _TPE_drawDebugPixel(p.x a b,p.y c d,camView.x,camView.y,color,drawFunc);
          dp(+,dx,+,dy) dp(+,dx,-,dy) dp(-,dx,+,dy) dp(-,dx,-,dy)
          dp(+,dy,+,dx) dp(+,dy,-,dx) dp(-,dy,+,dx) dp(-,dy,-,dx)
#undef dp
#undef SEGS
        }
      }
    }
  }
#undef Z_LIMIT
}

TPE_Vec3 TPE_envBox(TPE_Vec3 point, TPE_Vec3 center, TPE_Vec3 maxCornerVec,
  TPE_Vec3 rotation)
{
  point = TPE_pointRotate(TPE_vec3Minus(point,center),
    TPE_rotationInverse(rotation));

  return TPE_vec3Plus(center,TPE_pointRotate(TPE_envAABox(point,TPE_vec3(0,0,0),
    maxCornerVec),rotation));
}

TPE_Vec3 TPE_envAABox(TPE_Vec3 point, 
                      TPE_Vec3 center, 
                      TPE_Vec3 maxCornerVec)
{
  TPE_Vec3 shifted = TPE_vec3Minus(point,center);
  signed char sign[3] = {1, 1, 1};
  unsigned char region;

  if (shifted.x < 0)
  {
    shifted.x *= -1;
    sign[0] = -1;
  }

  if (shifted.y < 0)
  {
    shifted.y *= -1;
    sign[1] = -1;
  }

  if (shifted.z < 0)
  {
    shifted.z *= -1;
    sign[2] = -1;
  }

  region =
    (shifted.x > maxCornerVec.x) |
    ((shifted.y > maxCornerVec.y) << 1) |
    ((shifted.z > maxCornerVec.z) << 2);

  switch (region)
  {
#define align(c,i) point.c = center.c + sign[i] * maxCornerVec.c

    case 0x01: align(x,0); break;
    case 0x02: align(y,1); break;
    case 0x04: align(z,2); break;

    case 0x03: align(x,0); align(y,1); break;
    case 0x05: align(x,0); align(z,2); break;
    case 0x06: align(y,1); align(z,2); break;

    case 0x07: align(x,0); align(y,1); align(z,2); break; 
    default: break;

#undef align
  }

  return point;
}

TPE_Vec3 TPE_envAABoxInside(TPE_Vec3 point, 
                            TPE_Vec3 center, 
                            TPE_Vec3 size)
{
  TPE_Vec3 shifted, a, b;
  signed char sx, sy, sz;
  size.x /= 2;
  size.y /= 2;
  size.z /= 2;

  shifted = TPE_vec3Minus(point,center);

  a = TPE_vec3Minus(size,shifted);
  b = TPE_vec3Plus(shifted,size);

  sx = 1, 
  sy = 1, 
  sz = 1;

  if (b.x < a.x)
  {
    a.x = b.x;
    sx = -1;
  }

  if (b.y < a.y)
  {
    a.y = b.y;
    sy = -1;
  }

  if (b.z < a.z)
  {
    a.z = b.z;
    sz = -1;
  }

  if (a.x < 0 || a.y < 0 || a.z < 0)
    return point;

  if (a.x < a.y)
  {
    if (a.x < a.z)
      point.x = center.x + sx * size.x;
    else
      point.z = center.z + sz * size.z;
  }
  else
  {
    if (a.y < a.z)
      point.y = center.y + sy * size.y;
    else
      point.z = center.z + sz * size.z;
  }

  return point;
}

TPE_Vec3 TPE_envSphereInside(TPE_Vec3 point, TPE_Vec3 center, TPE_Unit radius)
{
  TPE_Vec3 shifted = TPE_vec3Minus(point,center);

  TPE_Unit l = TPE_LENGTH(shifted);

  if (l >= radius)
    return point;
  else if (l < 0)
    return TPE_vec3(center.x + radius,center.y,center.z);

  TPE_vec3Normalize(&shifted);
 
  return TPE_vec3Plus(center,TPE_vec3Times(shifted,radius));
}

TPE_Vec3 TPE_envSphere(TPE_Vec3 point, TPE_Vec3 center, TPE_Unit radius)
{
  TPE_Vec3 dir = TPE_vec3Minus(point,center);

  TPE_Unit l = TPE_LENGTH(dir);

  if (l <= radius)
    return point;

  dir.x = (dir.x * radius) / l;
  dir.y = (dir.y * radius) / l;
  dir.z = (dir.z * radius) / l;

  return TPE_vec3Plus(center,dir);
}

TPE_Vec3 TPE_envHalfPlane(TPE_Vec3 point, 
                          TPE_Vec3 center, 
                          TPE_Vec3 normal)
{
  TPE_Vec3 point2 = TPE_vec3Minus(point,center);
  TPE_Unit l;

  TPE_Unit tmp =
    point2.x * normal.x + point2.y * normal.y + point2.z * normal.z;

  if (tmp < 0)
    return point;

  l = TPE_LENGTH(normal);

  tmp /= l;

  normal.x = (normal.x * TPE_F) / l;
  normal.y = (normal.y * TPE_F) / l;
  normal.z = (normal.z * TPE_F) / l;

  return TPE_vec3Minus(point,
    TPE_vec3Times(normal,tmp));
}

unsigned char TPE_checkOverlapAABB(TPE_Vec3 v1Min, TPE_Vec3 v1Max, TPE_Vec3 v2Min,
  TPE_Vec3 v2Max)
{
  TPE_Unit dist;

#define test(c) \
  dist = v1Min.c + v1Max.c - v2Max.c - v2Min.c; \
  if (dist < 0) dist *= -1; \
  if (dist > v1Max.c - v1Min.c + v2Max.c - v2Min.c) return 0;

  test(x)
  test(y)
  test(z)

#undef test

  return 1;
}

void TPE_bodyGetAABB(const TPE_Body *body, 
                     TPE_Vec3 *vMin, 
                     TPE_Vec3 *vMax)
{
  TPE_Unit js;
  unsigned short i;

  *vMin = body->joints[0].position;
  *vMax = *vMin;

  js = TPE_JOINT_SIZE(body->joints[0]);

  vMin->x -= js;
  vMin->y -= js;
  vMin->z -= js;

  vMax->x += js;
  vMax->y += js;
  vMax->z += js;

  for (i = 1; i < body->jointCount; ++i)
  {
    TPE_Unit v;
  
    js = TPE_JOINT_SIZE(body->joints[i]);
  
#define test(c) \
  v = body->joints[i].position.c - js; \
  if (v < vMin->c) \
    vMin->c = v; \
  v += 2 * js; \
  if (v > vMax->c) \
    vMax->c = v;

    test(x)
    test(y)
    test(z)

#undef test
  }
}

void TPE_jointPin(TPE_Joint *joint, TPE_Vec3 position)
{
  joint->position = position;
  joint->velocity[0] = 0;
  joint->velocity[1] = 0;
  joint->velocity[2] = 0;
}

TPE_Vec3 TPE_pointRotate(TPE_Vec3 point, TPE_Vec3 rotation)
{
  _TPE_vec2Rotate(&point.y,&point.x,rotation.z);
  _TPE_vec2Rotate(&point.z,&point.y,rotation.x);
  _TPE_vec2Rotate(&point.x,&point.z,rotation.y);

  return point;
}

TPE_Vec3 TPE_rotationInverse(TPE_Vec3 rotation)
{
  /* If r1 = (X,Y,Z) is rotation in convention ABC then r1^-1 = (-X,-Y,-Z) in
     convention CBA is its inverse rotation. We exploit this, i.e. we rotate
     forward/right vectors in opposite axis order and then turn the result
     into normal rotation/orientation. 
  */

  TPE_Vec3 f = TPE_vec3(0,0,TPE_F);
  TPE_Vec3 r = TPE_vec3(TPE_F,0,0);

  rotation.x *= -1;
  rotation.y *= -1;
  rotation.z *= -1;

  _TPE_vec2Rotate(&f.x,&f.z,rotation.y);
  _TPE_vec2Rotate(&f.z,&f.y,rotation.x);
  _TPE_vec2Rotate(&f.y,&f.x,rotation.z);

  _TPE_vec2Rotate(&r.x,&r.z,rotation.y);
  _TPE_vec2Rotate(&r.z,&r.y,rotation.x);
  _TPE_vec2Rotate(&r.y,&r.x,rotation.z);

  return TPE_rotationFromVecs(f,r);
}

TPE_Vec3 TPE_rotationRotateByAxis(TPE_Vec3 rotation, TPE_Vec3 rotationByAxis)
{
  TPE_Vec3 f = TPE_pointRotate(TPE_vec3(0,0,TPE_F),rotation);
  TPE_Vec3 r = TPE_pointRotate(TPE_vec3(TPE_F,0,0),rotation);

  TPE_Unit a = TPE_LENGTH(rotationByAxis);
  TPE_vec3Normalize(&rotationByAxis);

  f = _TPE_rotateByAxis(f,rotationByAxis,a);
  r = _TPE_rotateByAxis(r,rotationByAxis,a);

  return TPE_rotationFromVecs(f,r);
}

TPE_Unit TPE_keepInRange(TPE_Unit x, TPE_Unit xMin, TPE_Unit xMax)
{
  return x > xMin ? (x < xMax ? x : xMax) : xMin;
}

TPE_Vec3 TPE_vec3KeepWithinDistanceBand(TPE_Vec3 point, TPE_Vec3 center,
  TPE_Unit minDistance, TPE_Unit maxDistance)
{
  TPE_Vec3 toPoint = TPE_vec3Minus(point,center);

  TPE_Unit l = TPE_LENGTH(toPoint);

  if (l <= maxDistance)
  {
    if (l >= minDistance)
      return point;
 
    l = minDistance;
  }
  else
    l = maxDistance;

  return TPE_vec3Plus(center,
    TPE_vec3Times(TPE_vec3Normalized(toPoint),l));
}

TPE_Vec3 TPE_vec3KeepWithinBox(TPE_Vec3 point, TPE_Vec3 boxCenter,
  TPE_Vec3 boxMaxVect)
{
  point.x = TPE_keepInRange(point.x,
    boxCenter.x - boxMaxVect.x,boxCenter.x + boxMaxVect.x);

  point.y = TPE_keepInRange(point.y,
    boxCenter.y - boxMaxVect.y,boxCenter.y + boxMaxVect.y);

  point.z = TPE_keepInRange(point.z,
    boxCenter.z - boxMaxVect.z,boxCenter.z + boxMaxVect.z);

  return point;
}

TPE_Vec3 TPE_envInfiniteCylinder(TPE_Vec3 point, 
                                 TPE_Vec3 center, 
                                 TPE_Vec3 direction, 
                                 TPE_Unit radius)
{
  TPE_Unit l;

  TPE_Vec3 d = TPE_vec3Minus(point,center);
  d = TPE_vec3Minus(d,TPE_vec3Project(d,direction));

  l = TPE_LENGTH(d);

  if (l <= radius)
    return point;

  radius = l - radius;

  d.x = (d.x * radius) / l;
  d.y = (d.y * radius) / l;
  d.z = (d.z * radius) / l;
    
  return TPE_vec3Minus(point,d);
}

TPE_Vec3 TPE_envCylinder(TPE_Vec3 point, 
                         TPE_Vec3 center, 
                         TPE_Vec3 direction,
                         TPE_Unit radius)
{
  TPE_Vec3 projected;
  TPE_Unit lDir, lDiff;

  point = TPE_vec3Minus(point,center);

  projected = TPE_vec3Project(point,direction);

  point = TPE_envInfiniteCylinder(point,TPE_vec3(0,0,0),direction,radius);

  lDir = TPE_nonZero(TPE_LENGTH(direction));

  lDiff = TPE_LENGTH(projected) - lDir;

  if (lDiff > 0)
  {
    direction.x = (direction.x * lDiff) / lDir;
    direction.y = (direction.y * lDiff) / lDir;
    direction.z = (direction.z * lDiff) / lDir;

    point = (TPE_vec3Dot(projected,direction)) >= 0 ?
      TPE_vec3Minus(point,direction) : TPE_vec3Plus(point,direction);
  }

  return TPE_vec3Plus(center,point);
}

TPE_Vec3 TPE_fakeSphereRotation(TPE_Vec3 position1, 
                                TPE_Vec3 position2,
                                TPE_Unit radius)
{
  TPE_Vec3 m;
  TPE_Unit l, d;

  m.x = position1.z - position2.z;
  m.y = 0;
  m.z = position2.x - position1.x;
    
  l = TPE_sqrt(m.x * m.x + m.z * m.z);

  if (l == 0)
    return TPE_vec3(0,0,0);

  d = (TPE_DISTANCE(position1,position2) * 
    TPE_F) / (radius * 4);

  m.x = (m.x * d) / l;
  m.z = (m.z * d) / l;
  
  return m;
}

TPE_Vec3 TPE_castEnvironmentRay(TPE_Vec3 rayPos, 
                                TPE_Vec3 rayDir,
                                TPE_ClosestPointFunction environment, 
                                TPE_Unit insideStepSize,
                                TPE_Unit rayMarchMaxStep, 
                                unsigned int maxSteps)
{
  TPE_Vec3 p = rayPos;
  TPE_Vec3 p2 = environment(rayPos,rayMarchMaxStep);
  TPE_Unit totalD = 0;
  unsigned char found;
  unsigned int i;
  TPE_Vec3 pTest;

  TPE_vec3Normalize(&rayDir);

  found = 0; /* 0 = nothing found, 1 = out/in found, 2 = in/out found */

  if (p2.x != p.x || p2.y != p.y || p2.z != p.z)
  {
    /* outside ray: ray march */

    for (i = 0; i < maxSteps; ++i)
    {
      TPE_Unit d = TPE_DISTANCE(p,p2);

      if (d > rayMarchMaxStep)
        d = rayMarchMaxStep;

      totalD += d;

      p2 = TPE_vec3Plus(rayPos,TPE_vec3Times(rayDir,totalD));

      if (d == 0 || 
        (p2.x == p.x && p2.y == p.y && p2.z == p.z))
        return p2; /* point not inside env but dist == 0, ideal case */

      pTest = environment(p2,rayMarchMaxStep);

      if (pTest.x == p2.x && pTest.y == p2.y && pTest.z == p2.z)
      {
        /* stepped into env, will have to iterate */
        found = 1;
        break;
      }

      p = p2;
      p2 = pTest;
    }
  }
  else if (insideStepSize != 0)
  {
    /* inside ray: iterate by fixed steps */

    for (i = 0; i < maxSteps; ++i)
    {
      totalD += insideStepSize;

      p2 = TPE_vec3Plus(rayPos,TPE_vec3Times(rayDir,totalD));

      pTest = environment(p2,16);

      if (p2.x != pTest.x || p2.y != pTest.y || p2.z != pTest.z)
      {
        found = 2;
        break;
      }

      p = p2;
      p2 = pTest;
    }
  }

  if (found)
  {
    /* Here we've found two points (p, p2), each one the other side of the
       env surface. Now iterate (binary search) to find the exact surface
       pos. 
    */

    for (i = 0; i < 128; ++i) /* upper limit just in case */
    {
      TPE_Vec3 middle = TPE_vec3Plus(p,p2);
 
      middle.x /= 2;
      middle.y /= 2;
      middle.z /= 2;

      if ((middle.x == p.x && middle.y == p.y && middle.z == p.z) ||
        (middle.x == p2.x && middle.y == p2.y && middle.z == p2.z))
        break; /* points basically next to each other, don't continue */

      pTest = environment(middle,16); /* 16: just a small number */

      if ((found == 1) ==
        (pTest.x == middle.x && pTest.y == middle.y && pTest.z == middle.z))
        p2 = middle;
      else
        p = middle;
    }

    return (found == 1) ? p : p2;
  }

  return TPE_vec3(TPE_INFINITY,TPE_INFINITY,TPE_INFINITY);
}

TPE_Vec3 TPE_castBodyRay(TPE_Vec3 rayPos, 
                         TPE_Vec3 rayDir, 
                         short excludeBody,
                         const TPE_World *world, 
                         short *bodyIndex, 
                         short *jointIndex)
{
  unsigned short i, j;
  TPE_Unit js, d2;
  TPE_Vec3 i1, i2;

  TPE_Vec3 bestP = TPE_vec3(TPE_INFINITY,TPE_INFINITY,TPE_INFINITY);
  TPE_Unit bestD = TPE_INFINITY;

  if (bodyIndex != 0)
    *bodyIndex = -1;

  if (jointIndex != 0)
    *jointIndex = -1;

  TPE_vec3Normalize(&rayDir);

  for (i = 0; i < world->bodyCount; ++i)
  {
    TPE_Vec3 c, p;
    TPE_Unit r, d;

    TPE_bodyGetFastBSphere(&world->bodies[i],&c,&r);

    c = TPE_vec3Minus(c,rayPos);
    p = TPE_vec3ProjectNormalized(c,rayDir);

    if (TPE_vec3Dot(p,rayDir) >= 0) /* point is in ray's forward dir? */
    {
      d = TPE_DISTANCE(p,c);

      if (d <= r)
      {
        /* bounding sphere hit, now check all joints:
        */


        const TPE_Joint *joint = world->bodies[i].joints;

        for (j = 0; j < world->bodies[i].jointCount; ++j)
        {
          c = joint->position;
          c = TPE_vec3Minus(c,rayPos);
          p = TPE_vec3ProjectNormalized(c,rayDir);

          if (TPE_vec3Dot(p,rayDir) >= 0)
          {
            d = TPE_DISTANCE(p,c);
             js = TPE_JOINT_SIZE(*joint);

            if (d <= js)
            {
              /* joint hit, compute exact coordinates:
              */


              if (bodyIndex != 0)
                *bodyIndex = i;

              if (jointIndex != 0)
                *jointIndex = j;

              c = TPE_vec3Times(rayDir,TPE_sqrt(js * js - d * d));
              /* ^ offset vector to two intersections */
              p = TPE_vec3Plus(p,rayPos);

              
                i1 = TPE_vec3Plus(p,c); /* intersection points */
                i2 = TPE_vec3Minus(p,c);

              d = TPE_DISTANCE(rayPos,i1);
              d2 = TPE_DISTANCE(rayPos,i2);

              if (d2 < d) /* take the closer one */
              {
                d = d2;
                i1 = i2;
              }

              if (d < bestD)
              {
                bestD = d;
                bestP = i1;
              }
            }
          }

          joint++;
        }
      }
    }
  }

  return bestP;
}

void TPE_worldDeactivateAll(TPE_World *world)
{
  unsigned short i;

  for (i = 0; i < world->bodyCount; ++i)
    TPE_bodyDeactivate(&world->bodies[i]);
}

void TPE_worldActivateAll(TPE_World *world)
{
  unsigned short i;

  for (i = 0; i < world->bodyCount; ++i)
    TPE_bodyActivate(&world->bodies[i]);
}

TPE_Unit TPE_worldGetNetSpeed(const TPE_World *world)
{
  TPE_Unit result = 0;
  unsigned short i;

  for (i = 0; i < world->bodyCount; ++i)
    result += TPE_bodyGetNetSpeed(world->bodies + i);

  return result;
}

TPE_Vec3 TPE_bodyGetLinearVelocity(const TPE_Body *body)
{
  TPE_Vec3 r = TPE_vec3(0,0,0);
  unsigned short i;

  for (i = 0; i < body->jointCount; ++i)
  {
    TPE_UnitReduced *v = body->joints[i].velocity;
    r = TPE_vec3Plus(r,TPE_vec3(v[0],v[1],v[2]));
  }

  r.x /= body->jointCount;
  r.y /= body->jointCount;
  r.z /= body->jointCount;
    
  return r;
}

TPE_Unit TPE_abs(TPE_Unit x)
{
  return x >= 0 ? x : (-1 * x);
}

TPE_Unit TPE_max(TPE_Unit a, TPE_Unit b)
{
  return (a > b) ? a : b;
}

TPE_Unit TPE_min(TPE_Unit a, TPE_Unit b)
{
  return (a < b) ? a : b;
}

TPE_Vec3 TPE_envAATriPrism(TPE_Vec3 point, 
                           TPE_Vec3 center,
                           const TPE_Unit sides[6], 
                           TPE_Unit depth, 
                           unsigned char direction)
{
  unsigned char i;
  point = TPE_vec3Minus(point,center);

  if (direction == 1)
  {
    TPE_Unit tmp = point.z;
    point.z = point.y;
    point.y = tmp;
  }
  else if (direction == 2)
  {
    TPE_Unit tmp = point.z;
    point.z = point.x;
    point.x = tmp;
  }

  depth /= 2;

  if (point.z > depth)
    point.z = depth;
  else
  {
    depth *= -1;

    if (point.z < depth)
      point.z = depth;
  }

  for (i = 0; i < 6; i += 2)
  {
    unsigned char i2 = i < 4 ? i + 2 : 0;

    TPE_Vec3 p =
      TPE_envHalfPlane(point,TPE_vec3(sides[i],sides[i + 1],0),
      TPE_vec3(sides[i2 + 1] - sides[i + 1],sides[i] - sides[i2],0));

    if (p.x != point.x || p.y != point.y)
    {
      point = p;

      if ( /* dot product to determine which side the point is on */
        (sides[i2] - sides[i]) * (point.x - sides[i]) +
        (sides[i2 + 1] - sides[i + 1]) * (point.y - sides[i + 1]) < 0)
      {
        point.x = sides[i]; point.y = sides[i + 1];
      }
      else if ( /* same but for the other vertex */
        (sides[i] - sides[i2]) * (point.x - sides[i2]) +
        (sides[i + 1] - sides[i2 + 1]) * (point.y - sides[i2 + 1]) < 0)
      {
        point.x = sides[i2]; point.y = sides[i2 + 1];
      }

      break;
    }
  }

  if (direction == 1)
  {
    TPE_Unit tmp = point.z;
    point.z = point.y;
    point.y = tmp;
  }
  else if (direction == 2)
  {
    TPE_Unit tmp = point.z;
    point.z = point.x;
    point.x = tmp;
  }

  return TPE_vec3Plus(point,center);
}

TPE_Vec3 TPE_envGround(TPE_Vec3 point, TPE_Unit height)
{
  if (point.y > height)
    point.y = height;

  return point;
}

unsigned int _TPE_hash(unsigned int n)
{
  /* parameters found by hash-prospector project */
  n = 250009959 * (n ^ (n >> 17));
  n = 2626308659 * (n ^ (n >> 15));
  return n ^ (n >> 16);
}

unsigned int TPE_jointHash(const TPE_Joint *joint)
{
  unsigned int 
    r = _TPE_hash(joint->position.x);
    r = _TPE_hash(r ^ joint->position.y);
    r = _TPE_hash(r ^ joint->position.z);
    r = _TPE_hash(r ^
      (((unsigned int) joint->velocity[0]) |
      (((unsigned int) joint->velocity[1]) << 16)));
    r = _TPE_hash(r ^ 
      (((unsigned int) joint->velocity[2]) |
      ((unsigned int) joint->sizeDivided)));
  
  return r;
}

unsigned int TPE_connectionHash(const TPE_Connection *connection)
{
  return _TPE_hash(
   ((unsigned int) connection->length) |
   (((unsigned int) connection->joint1) << 16) |
   (((unsigned int) connection->joint2) << 24));
}

unsigned int TPE_bodyHash(const TPE_Body *body)
{
  unsigned char i;

  unsigned int r = _TPE_hash(
    ((unsigned int) body->jointMass) |
    (((unsigned int) body->flags) << 16) |
    (((unsigned int) body->deactivateCount) << 24)) ^
      _TPE_hash(
    ((unsigned int) body->friction) |
    (((unsigned int) body->elasticity) << 16));

  for (i = 0; i < body->jointCount; ++i)
    r = _TPE_hash(r ^ TPE_jointHash(&body->joints[i]));

  for (i = 0; i < body->connectionCount; ++i)
    r = _TPE_hash(r ^ TPE_connectionHash(&body->connections[i]));

  return r;
}

unsigned int TPE_worldHash(const TPE_World *world)
{
  unsigned int r = 0;
  unsigned char i;

  for (i = 0; i < world->bodyCount; ++i)
    r = _TPE_hash(r ^ TPE_bodyHash(&world->bodies[i]));

  return r;
}

void TPE_bodyMoveTo(TPE_Body *body, 
                    TPE_Vec3 position)
{
  unsigned char i;
  position = TPE_vec3Minus(position,TPE_bodyGetCenterOfMass(body));

  for (i = 0; i < body->jointCount; ++i)
    body->joints[i].position = TPE_vec3Plus(body->joints[i].position,position);
}

unsigned char TPE_testClosestPointFunction(TPE_ClosestPointFunction f,
                                           TPE_Vec3 cornerFrom, 
                                           TPE_Vec3 cornerTo, 
                                           unsigned char gridResolution,
                                           TPE_UnitReduced allowedError, 
                                           TPE_Vec3 *errorPoint)
{
  TPE_Vec3 p, p2, p3, p4;
  unsigned short x, y, z;
  unsigned char i, xx ,yy ,zz;
  TPE_Unit d;

  cornerTo = TPE_vec3Minus(cornerTo,cornerFrom);

  for (z = 0; z < gridResolution; ++z)
  {
    p.z = cornerFrom.z + (z * cornerTo.z) / gridResolution;

    for (y = 0; y < gridResolution; ++y)
    {
      p.y = cornerFrom.y + (y * cornerTo.y) / gridResolution;

      for (x = 0; x < gridResolution; ++x)
      {
        p.x = cornerFrom.x + (x * cornerTo.x) / gridResolution;

        p2 = f(p,TPE_INFINITY);

        if (p.x != p2.x || p.y != p2.y || p.z != p2.z) /* only test outside */
        {
          /* 1st try to approach the closest point and see if it stays the same:
          */

          p3 = p;

          for (i = 0; i < 3; ++i)
          {
            p3 =
              TPE_vec3((p3.x + p2.x) / 2,(p3.y + p2.y) / 2,(p3.z + p2.z) / 2);

            p4 = f(p3,TPE_INFINITY);

            if (TPE_abs(p4.x - p2.x) + TPE_abs(p4.y - p2.y) 
              + TPE_abs(p4.z - p2.z) > allowedError) /* taxicab dist. for speed */
            {
              if (errorPoint != 0)
                *errorPoint = p;

              return 0;
            }
          }
 
          /* now test 8 points inside the sphere of radius: 
          */

          d = TPE_DISTANCE(p,p2);

          p3.z = p.z - d / 2;
         
          for (zz = 0; zz < 2; ++zz)
          {
            p3.y = p.y - d / 2;

            for (yy = 0; yy < 2; ++yy)
            {
              p3.x = p.x - d / 2;

              for (zz = 0; zz < 2; ++zz)
              {
                if (TPE_DISTANCE(p,f(p3,TPE_INFINITY)) + allowedError < d)
                {
                  /* In the sphere of distance radius to the original point's
                     closest point we've gotten a closer point which should
                     never happen. 
                  */

                  if (errorPoint != 0)
                    *errorPoint = p;

                  return 0;
                }

                p3.x += d;
              }

              p3.y += d;
            }
 
            p3.z += d;
          }
        }
      }
    }
  }

  return 1;
}

TPE_Vec3 TPE_envLineSegment(TPE_Vec3 point, TPE_Vec3 a, TPE_Vec3 b)
{
  point = TPE_vec3Minus(point,a);

  b = TPE_vec3Minus(b,a);

  point = TPE_vec3Project(point,b);

  if (TPE_vec3Dot(point,b) < 0)
    point = TPE_vec3(0,0,0);
  else if (TPE_abs(point.x) + TPE_abs(point.y) + TPE_abs(point.z) >
    TPE_abs(b.x) + TPE_abs(b.y) + TPE_abs(b.z))
    point = b;

  point = TPE_vec3Plus(point,a);

  return point;
}

TPE_Vec3 TPE_envHeightmap(TPE_Vec3 point, 
                          TPE_Vec3 center, 
                          TPE_Unit gridSize,
                          TPE_Unit (*heightFunction)(int x, int y), 
                          TPE_Unit maxDist)
{
  TPE_Vec3 closestP;
  TPE_Unit closestD;
  unsigned short i;
  short startSquareX, startSquareY, squareX, squareY;
  unsigned char spiralDir = 1, j;
  unsigned short spiralStep = 1, spiralStepsLeft = 1;
  TPE_Vec3 bl, br, tl, tr;
  point = TPE_vec3Minus(point,center);

  closestP = TPE_vec3(TPE_INFINITY,TPE_INFINITY,TPE_INFINITY);
  closestD = TPE_INFINITY;

  startSquareX = point.x / gridSize - (point.x < 0);
  startSquareY = point.z / gridSize - (point.z < 0);

  squareX = startSquareX;
  squareY = startSquareY;

    /* 4 corners of the current square */
    bl = TPE_vec3(squareX * gridSize,heightFunction(squareX,squareY), squareY * gridSize);
    br = TPE_vec3(bl.x + gridSize,heightFunction(squareX + 1,squareY),bl.z);
    tl = TPE_vec3(bl.x,heightFunction(squareX,squareY + 1),bl.z + gridSize);
    tr = TPE_vec3(br.x,heightFunction(squareX + 1,squareY + 1),tl.z);

  for (i = 0; i < 1024; ++i) /* while (1) should work in theory but... */
  {
    if ((TPE_min(TPE_abs(squareX - startSquareX),
      TPE_abs(squareY - startSquareY)) - 1) * gridSize
      > TPE_min(maxDist,closestD))
      break; /* here we can no longer find the dist we're looking for => end */

    for (j = 0; j < 2; ++j) /* check the two triangles of the segment */
    {
      TPE_Vec3 testP = TPE_envHalfPlane(point,j == 0 ? bl : tr,
        TPE_vec3Normalized(j == 0 ?
          TPE_vec3Cross(TPE_vec3Minus(tl,bl),TPE_vec3Minus(br,bl)) :
          TPE_vec3Cross(TPE_vec3Minus(br,tr),TPE_vec3Minus(tl,tr))));

      TPE_Unit testD = TPE_DISTANCE(testP,point);

      if (testD < closestD)
      {
        if (j == 0 ? /* point is inside the triangle? */
          (testP.x >= bl.x && testP.z >= bl.z &&
            (testP.x - bl.x <= tl.z - testP.z)) :
          (testP.x <= tr.x && testP.z <= tr.z &&
            (testP.x - bl.x >= tl.z - testP.z)))
        {
          closestP = testP;
          closestD = testD;
        }
        else
        {
          /* point outside the triangle, check individual boundary sides */
#define testEdge(a,b) \
  testP = TPE_envLineSegment(point,a,b); testD = TPE_DISTANCE(testP,point); \
  if (testD < closestD) { closestP = testP; closestD = testD; }

          testEdge(j == 0 ? bl : tr,br)
          testEdge(j == 0 ? bl : tr,tl)
          testEdge(br,tl)

#undef testEdge
        }
      }
    }

    /* now step to another square, in spiralling way:
    */

    switch (spiralDir)
    {
      case 0: /* moving up */
        squareY++; 

        bl = tl; br = tr;
        tl = TPE_vec3(bl.x,heightFunction(squareX,squareY + 1),bl.z + gridSize);
        tr = TPE_vec3(br.x,heightFunction(squareX + 1,squareY + 1),bl.z 
          + gridSize);

        break;

      case 1: /* moving right */
        squareX++;

        bl = br; tl = tr;
        tr = TPE_vec3(tl.x + gridSize,heightFunction(squareX + 1,squareY + 1),
          tl.z);
        br = TPE_vec3(bl.x + gridSize,heightFunction(squareX + 1,squareY),bl.z);

        break;

      case 2: /* moving down */
        squareY--;

        tl = bl; tr = br;
        bl = TPE_vec3(tl.x,heightFunction(squareX,squareY),tl.z - gridSize);
        br = TPE_vec3(tr.x,heightFunction(squareX + 1,squareY),tr.z - gridSize);

        break;

      case 3: /* moving left */
        squareX--;

        br = bl; tr = tl;
        tl = TPE_vec3(tr.x - gridSize,heightFunction(squareX,squareY + 1),tr.z);
        bl = TPE_vec3(br.x - gridSize,heightFunction(squareX,squareY),br.z);

        break;

      default: break;
    }

    spiralStepsLeft--;

    if (spiralStepsLeft == 0)
    {
      spiralDir = spiralDir != 0 ? spiralDir - 1 : 3;

      if (spiralDir == 3 || spiralDir == 1)
        spiralStep++;

      spiralStepsLeft = spiralStep;
    }
  }

  return TPE_vec3Plus(closestP,center);
}

TPE_Vec3 TPE_envCone(TPE_Vec3 point, 
                     TPE_Vec3 center, 
                     TPE_Vec3 direction,
                     TPE_Unit radius)
{
  TPE_Unit dist = TPE_LENGTH(point);
  TPE_Unit x;
  point = TPE_vec3Minus(point,center);

  if (TPE_vec3Dot(point,direction) <= 0)
  {
    /* underneath the cone
    */

    direction.x *= -1;
    direction.y *= -1;
    direction.z *= -1;

    point = TPE_envHalfPlane(point,TPE_vec3(0,0,0),direction);

    if (dist > radius)
    {
      point.x = (point.x * radius) / dist;
      point.y = (point.y * radius) / dist;
      point.z = (point.z * radius) / dist;
    }
  }
  else
  {
    TPE_Unit height = TPE_LENGTH(direction);

    TPE_Vec3 helper = TPE_vec3Project(point,direction);
    TPE_Unit y = TPE_LENGTH(helper);

    helper = TPE_vec3Minus(point,helper);

    x = TPE_LENGTH(helper);

    if (x < 20)
    {
      /* for such small distance big numeric errors occur in the other branch */
      if (y >= height)
        point = direction;
    }
    else
    {
      TPE_Unit scaledRadius = radius - ((y * radius) / height);

      if (y > height || x > scaledRadius) // outside?
      {
        if (x <= 0)
        {
          TPE_LOG("WARNING: arithmetic error in envCone (library bug)");
          x = 1; /* shouldn't happen but just in case, to prevent div by zero */
        }

        helper.x = (helper.x * radius) / x;
        helper.y = (helper.y * radius) / x;
        helper.z = (helper.z * radius) / x;

        point = TPE_envLineSegment(point,helper,direction); 
      }
    }
  }

  return TPE_vec3Plus(point,center);
}

static unsigned char TPE_bodyIsActive(const TPE_Body *body)
{
  return !(body->flags & TPE_BODY_FLAG_DEACTIVATED);
}

#endif /* guard */