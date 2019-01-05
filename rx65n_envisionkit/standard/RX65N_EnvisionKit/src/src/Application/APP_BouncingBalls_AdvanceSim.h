/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2017  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

***** emWin - Graphical user interface for embedded applications *****
emWin is protected by international copyright laws.   Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with a license and should not be re-
distributed in any way. We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : APP_BouncingBalls_AdvanceSim.h
Purpose     : Routines to be compiled with different FPU settings
----------------------------------------------------------------------
*/

/*********************************************************************
*
*       Static code: Helper(s)
*
**********************************************************************
*/
/*********************************************************************
*
*       _Square
*/
static float _Square(float x) { 
  return x * x;
}

/*********************************************************************
*
*       _GetRandomNumber
*/
static float _GetRandomNumber(float min, float max) {
  return (max - min) * rand() / RAND_MAX + min;
}

#ifdef WIN32
static size_t _AllocatedBytes;
#endif

/*********************************************************************
*
*       Static code: VECTOR
*
**********************************************************************
*/
/*********************************************************************
*
*       _VECTOR_Create
*/
static VECTOR * _VECTOR_Create(float x, float y) {
  VECTOR * pVectorNew;
  
  pVectorNew = (VECTOR *)_Calloc(sizeof(VECTOR), 1);
  pVectorNew->x = x;
  pVectorNew->y = y;
  return pVectorNew;
}

/*********************************************************************
*
*       _VECTOR_CreateCopyMinus
*/
static VECTOR * _VECTOR_CreateCopyMinus(VECTOR * pVector, VECTOR * pVector1) {
  return _VECTOR_Create(pVector->x - pVector1->x, pVector->y - pVector1->y);
}

/*********************************************************************
*
*       _VECTOR_CreateCopyMult
*/
static VECTOR * _VECTOR_CreateCopyMult(VECTOR * pVector, float c) {
  return _VECTOR_Create(pVector->x * c, pVector->y * c);
}

/*********************************************************************
*
*       _VECTOR_Delete
*/
static void _VECTOR_Delete(VECTOR * pVector) {
  _Free(pVector);
}

/*********************************************************************
*
*       _VECTOR_SetX
*/
static void _VECTOR_SetX(VECTOR * pVector, float x) {
  pVector->x = x;
}

/*********************************************************************
*
*       _VECTOR_SetY
*/
static void _VECTOR_SetY(VECTOR * pVector, float y) {
  pVector->y = y;
}

/*********************************************************************
*
*       _VECTOR_SetXY
*/
static void _VECTOR_SetXY(VECTOR * pVector, float x, float y) {
  pVector->x = x;
  pVector->y = y;
}

/*********************************************************************
*
*       _VECTOR_Magnitude
*/
static float _VECTOR_Magnitude(VECTOR * pVector) { 
  return sqrt(pVector->x * pVector->x + pVector->y * pVector->y);
}

/*********************************************************************
*
*       _VECTOR_CreateUnitVector
*/
static VECTOR * _VECTOR_CreateUnitVector(VECTOR * pVector) {
  VECTOR * pVectorNew;
  float    Mag;

  Mag = _VECTOR_Magnitude(pVector);
  if (Mag != 0.f) {
    pVectorNew = _VECTOR_Create(pVector->x / Mag, pVector->y / Mag);
  } else {
    pVectorNew = _VECTOR_Create(0.f, 0.f);
  }
  return pVectorNew;
}

/*********************************************************************
*
*       _VECTOR_DotProduct
*/
static float _VECTOR_DotProduct(VECTOR * pVector, VECTOR * pVector1) {
  float DotProduct;

  DotProduct = pVector->x * pVector1->x + pVector->y * pVector1->y;
  return DotProduct;
}

/*********************************************************************
*
*       Static code: WALLS
*
**********************************************************************
*/
/*********************************************************************
*
*       _WALLS_Create
*/
static WALLS * _WALLS_Create(float x1, float y1, float x2, float y2) {
  WALLS * pWall;
  
  pWall = (WALLS *)_Calloc(sizeof(WALLS), 1);
  pWall->x1 = x1;
  pWall->y1 = y1;
  pWall->x2 = x2;
  pWall->y2 = y2;
  return pWall;
}

/*********************************************************************
*
*       Static code: BALL
*
**********************************************************************
*/
/*********************************************************************
*
*       _BALL_Create
*/
static BALL * _BALL_Create(void) {
  BALL * pBall;
  
  pBall = (BALL *)_Calloc(sizeof(BALL), 1);
  pBall->m = 0.f;
  pBall->r = 0.f;
  return pBall;
}

/*********************************************************************
*
*       _BALL_AdvanceBallPosition
*/
static void _BALL_AdvanceBallPosition(BALL * pBall, const float dt) {
  _VECTOR_SetXY(&pBall->p, pBall->p.x + pBall->v.x * dt, pBall->p.y + pBall->v.y * dt);
}

/*********************************************************************
*
*        _BALL_DoBallGravity
*/
static void _BALL_DoBallGravity(BALL * pb0, BALL * pb1, const float dt, float g) {
  VECTOR * v_n;
  VECTOR * v_un;
  float    r, f, a0, a1, v0, v1;

  v_n  = _VECTOR_CreateCopyMinus(&pb0->p, &pb1->p);  // v_n = normal vec. - a vector normal to the collision surface
  v_un = _VECTOR_CreateUnitVector(v_n);              // unit normal vector
  r = _VECTOR_Magnitude(v_n);
  f = g * (pb0->m * pb1->m) / (r * r);
  a0 = f / pb0->m;
  a1 = f / pb1->m;
  v0 = a0 * dt;
  v1 = a1 * dt;
  _VECTOR_SetXY(&pb0->v, pb0->v.x - v_un->x * v0, pb0->v.y - v_un->y * v0);
  _VECTOR_SetXY(&pb1->v, pb1->v.x + v_un->x * v1, pb1->v.y + v_un->y * v1);
  _VECTOR_Delete(v_n);
  _VECTOR_Delete(v_un);
}

/*********************************************************************
*
*        _BALL_DoGroundGravity
*/
static void _BALL_DoGroundGravity(BALL * pb, const float dt, float g) {
  _VECTOR_SetY(&pb->v, pb->v.y + g * dt);
}

/*********************************************************************
*
*       Static code: COLLISION
*
**********************************************************************
*/
/*********************************************************************
*
*       _COLLISION_Reset
*/
static void _COLLISION_Reset(COLLISION * pCollision) {
  pCollision->CollisionType   = TYPE_NONE;
  pCollision->WhichWall       = WALL_NONE;
  pCollision->TimeToCollision = 0.f;
}

/*********************************************************************
*
*       _COLLISION_SetCollisionWithWall
*/
static void _COLLISION_SetCollisionWithWall(COLLISION * pCollision, const float t, int WhichWall) {
  pCollision->CollisionType   = TYPE_WALL;
  pCollision->WhichWall       = WhichWall;
  pCollision->TimeToCollision = t;
}

/*********************************************************************
*
*       _COLLISION_SetCollisionWithBall
*/
static void _COLLISION_SetCollisionWithBall(COLLISION * pCollision, const float t) {
  pCollision->CollisionType   = TYPE_BALL;
  pCollision->WhichWall       = WALL_NONE;
  pCollision->TimeToCollision = t;
}

/*********************************************************************
*
*       _COLLISION_Ball1HasCollision
*/
static int _COLLISION_Ball1HasCollision(COLLISION * pCollision) {
  return pCollision->CollisionType != TYPE_NONE;
}

/*********************************************************************
*
*       _COLLISION_Ball1HasCollisionWithWall
*/
static int _COLLISION_Ball1HasCollisionWithWall(COLLISION * pCollision) {
  return pCollision->CollisionType == TYPE_WALL;
}

/*********************************************************************
*
*       _COLLISION_Ball1HasCollisionWithBall
*/
static int _COLLISION_Ball1HasCollisionWithBall(COLLISION * pCollision) {
  return pCollision->CollisionType == TYPE_BALL;
}

/*********************************************************************
*
*        _COLLISION_FindTimeUntilTwoBallsCollide
*
* Function description:
*   Finds the time until two specified balls collide. If they don't collide,
*   the returned Collision will indicate that. If the balls are overlapping
*   a collision is NOT detected.
*/
static COLLISION _COLLISION_FindTimeUntilTwoBallsCollide(const BALL * pb1, const BALL * pb2) {
  COLLISION clsn = {0};
  float     a, b, c, det, t;
  
  // Compute parts of quadratic formula
  //
  // a = (v2x - v1x) ^ 2 + (v2y - v1y) ^ 2
  //
  a = _Square(pb2->v.x - pb1->v.x) + _Square(pb2->v.y - pb1->v.y);
  //
  // b = 2 * ((x20 - x10) * (v2x - v1x) + (y20 - y10) * (v2y - v1y))
  //
  b = 2.f * ((pb2->p.x - pb1->p.x) * (pb2->v.x - pb1->v.x) + (pb2->p.y - pb1->p.y) * (pb2->v.y - pb1->v.y));
  //
  // c = (x20 - x10) ^ 2 + (y20 - y10) ^ 2 - (r1 + r2) ^ 2
  //
  c = _Square(pb2->p.x - pb1->p.x) + _Square(pb2->p.y - pb1->p.y) - _Square(pb1->r + pb2->r);
  //
  // Determinant = b^2 - 4ac
  //
  det = _Square(b) - 4 * a * c;
  if (a != 0.f) {                     // If a == 0 then v2x==v1x and v2y==v1y and there will be no collision
    t = (-b - sqrt(det)) / (2. * a);  // Quadratic formula. t = time to collision
    if (t >= 0.) {                    // If collision occurs...
      _COLLISION_SetCollisionWithBall(&clsn, t);
    }
  }
  return clsn;
}

/*********************************************************************
*
*        _COLLISION_FindTimeUntilBallCollidesWithWall
*
* Function description:
*   Finds time until specified ball collides with any wall. If they
*   don't collide, the returned Collision indicates that. If there
*   will be collisions with more than one wall, this function returns
*   the earliest collision.
*
* IMPORTANT: This function assumes that the ball is bounded within 
*   the specified walls.
*/
static COLLISION _COLLISION_FindTimeUntilBallCollidesWithWall(const BALL * pb, const WALLS * pw) {
  COLLISION clsn = {0};
  float     timeToCollision;
  float     t;
  int       whichWall;
  
  timeToCollision = 0.f;
  whichWall       = WALL_NONE;
  //
  // Check for collision with wall X1
  //
  if (pb->v.x < 0.f) {
    t = (pb->r - pb->p.x + pw->x1) / pb->v.x;
    if (t >= 0.f) {  // If t < 0 then ball is headed away from wall
      timeToCollision = t;
      whichWall = WALL_X1;
    }
  }
  //
  // Check for collision with wall Y1
  //
  if (pb->v.y < 0.f) {
    t = (pb->r - pb->p.y + pw->y1) / pb->v.y;
    if (t >= 0.f) {
      if (whichWall == WALL_NONE || t < timeToCollision) {
        timeToCollision = t;
        whichWall = WALL_Y1;
      }
    }
  }
  //
  // Check for collision with wall X2
  //
  if (pb->v.x > 0.f) {
    t = (pw->x2 - pb->r - pb->p.x) / pb->v.x;
    if (t >= 0.f) {
      if (whichWall == WALL_NONE || t < timeToCollision) {
        timeToCollision = t;
        whichWall = WALL_X2;
      }
    }
  }
  //
  // Check for collision with wall Y2
  //
  if (pb->v.y > 0.f) {
    t = (pw->y2 - pb->r - pb->p.y) / pb->v.y;
    if (t >= 0.f) {
      if (whichWall == WALL_NONE || t < timeToCollision) {
        timeToCollision = t;
        whichWall = WALL_Y2;
      }
    }
  }
  //
  // Setup Collision return value
  //
  if (whichWall != WALL_NONE) {  // If there is a collision...
    _COLLISION_SetCollisionWithWall(&clsn, timeToCollision, whichWall);
  }
  return clsn;
}

/*********************************************************************
*
*        _COLLISION_DoElasticCollisionTwoBalls
*
* Function description:
*   Updates the velocities of b1 and b2 to reflect the effect of an elastic
*   collision between the two. IMPORTANT: This function does NOT check the
*   positions of the balls to see if they're actually colliding. It just
*   assumes that they are. Use findTimeUntilTwoBallsCollide() to see
*   if the balls are colliding.
*/
static void _COLLISION_DoElasticCollisionTwoBalls(BALL * pb1, BALL * pb2) {
  VECTOR * v_n;
  VECTOR * v_un;
  VECTOR * v_ut;
  VECTOR * v_v1nPrime;
  VECTOR * v_v1tPrime;
  VECTOR * v_v2nPrime;
  VECTOR * v_v2tPrime;
  float    v1n, v1t, v2n, v2t;
  float    v1tPrime, v2tPrime;
  float    v1nPrime, v2nPrime;

  //
  // Avoid division by zero below in computing new normal velocities
  // Doing a collision where both balls have no mass makes no sense anyway
  //
  if ((pb1->m == 0.f) && (pb2->m == 0.f)) {
    return;
  }
  //
  // Compute unit normal and unit tangent vectors
  //
  v_n  = _VECTOR_CreateCopyMinus(&pb2->p, &pb1->p);  // v_n = normal vec. - a vector normal to the collision surface
  v_un = _VECTOR_CreateUnitVector(v_n);              // unit normal vector
  v_ut = _VECTOR_Create(-v_un->y, v_un->x);          // unit tangent vector
  //
  // Compute scalar projections of velocities onto v_un and v_ut
  //
  v1n = _VECTOR_DotProduct(v_un, &pb1->v);  // Dot product
  v1t = _VECTOR_DotProduct(v_ut, &pb1->v);
  v2n = _VECTOR_DotProduct(v_un, &pb2->v);
  v2t = _VECTOR_DotProduct(v_ut, &pb2->v);
  //
  // Compute new tangential velocities
  //
  v1tPrime = v1t;  // Note: in reality, the tangential velocities do not change after the collision
  v2tPrime = v2t;
  //
  // Compute new normal velocities using one-dimensional elastic collision equations in the normal direction
  // Division by zero avoided. See early return above.
  //
  v1nPrime = (v1n * (pb1->m - pb2->m) + 2.f * pb2->m * v2n) / (pb1->m + pb2->m);
  v2nPrime = (v2n * (pb2->m - pb1->m) + 2.f * pb1->m * v1n) / (pb1->m + pb2->m);
  //
  // Compute new normal and tangential velocity vectors
  //
  v_v1nPrime = _VECTOR_CreateCopyMult(v_un, v1nPrime);  // Multiplication by a scalar
  v_v1tPrime = _VECTOR_CreateCopyMult(v_ut, v1tPrime);
  v_v2nPrime = _VECTOR_CreateCopyMult(v_un, v2nPrime);
  v_v2tPrime = _VECTOR_CreateCopyMult(v_ut, v2tPrime);
  //
  // Set new velocities in x and y coordinates
  //
  _VECTOR_SetXY(&pb1->v, v_v1nPrime->x + v_v1tPrime->x, v_v1nPrime->y + v_v1tPrime->y);
  _VECTOR_SetXY(&pb2->v, v_v2nPrime->x + v_v2tPrime->x, v_v2nPrime->y + v_v2tPrime->y);
  //
  // CleanUp
  //
  _VECTOR_Delete(v_n);
  _VECTOR_Delete(v_un);
  _VECTOR_Delete(v_ut);
  _VECTOR_Delete(v_v1nPrime);
  _VECTOR_Delete(v_v1tPrime);
  _VECTOR_Delete(v_v2nPrime);
  _VECTOR_Delete(v_v2tPrime);
}

/*********************************************************************
*
*        _COLLISION_DoElasticCollisionWithWall
*/
static void _COLLISION_DoElasticCollisionWithWall(BALL * pb, int w) {
  switch (w) {
  case WALL_X1:
    _VECTOR_SetX(&pb->v, fabs(pb->v.x));
    break;
  case WALL_Y1:
    _VECTOR_SetY(&pb->v, fabs(pb->v.y));
    break;
  case WALL_X2:
    _VECTOR_SetX(&pb->v, -fabs(pb->v.x));
    break;
  case WALL_Y2:
    _VECTOR_SetY(&pb->v, -fabs(pb->v.y));
    break;
  }
}

/*********************************************************************
*
*       Static code: BALLSIM
*
**********************************************************************
*/
/*********************************************************************
*
*        _BALLSIM_ResetBalls
*/
static void _BALLSIM_ResetBalls(BALLSIM * pBallsim) {
  BALL * pBalli;
  BALL * pBalld;
  
  pBallsim->NextId        = 0;   // Reset ID counter
  pBallsim->MinArea       = 0.;
  pBallsim->MaxDiameter   = 0.;
  pBallsim->MaxCollisions = 10;  // This will be overwritten on the first call to addBall()
  //
  // Delete all balls
  //
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    pBalld = pBalli;
    pBalli = pBalli->pNext;
    _Free(pBalld);
  }
}

/*********************************************************************
*
*        _BALLSIM_Create
*/
static BALLSIM * _BALLSIM_Create(void) {
  BALLSIM * pBallsim;

  pBallsim = (BALLSIM *)_Calloc(sizeof(BALLSIM), 1);
  pBallsim->HasWalls = 0;
  pBallsim->MaxCollisionsPerBall = 10;
  _BALLSIM_ResetBalls(pBallsim);
  return pBallsim;
}

/*********************************************************************
*
*        _BALLSIM_AdvanceBallPositions
*/
static void _BALLSIM_AdvanceBallPositions(BALLSIM * pBallsim, const float dt) {
  BALL * pBalli;

  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    _BALL_AdvanceBallPosition(pBalli, dt);
    pBalli = pBalli->pNext;
  }
}

/*********************************************************************
*
*        BALLSSIM_FindEarliestCollisionOfTwoBalls
*/
static COLLISION BALLSSIM_FindEarliestCollisionOfTwoBalls(BALLSIM * pBallsim, BALL ** ppb1, BALL ** ppb2) {
  COLLISION earliestCollision;
  COLLISION c;
  BALL    * pBalli;
  BALL    * pBallj;
  
  _COLLISION_Reset(&earliestCollision);
  //
  // Compare each pair of balls. Index i runs from the first
  // ball up through the second-to-last ball. For each value of
  // i, index j runs from the ball after i up through the last ball.
  //
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    pBallj = pBallsim->pFirstBall;
    while (pBallj) {
      c = _COLLISION_FindTimeUntilTwoBallsCollide(pBalli, pBallj);
      if (_COLLISION_Ball1HasCollisionWithBall(&c)) {
        if (!_COLLISION_Ball1HasCollision(&earliestCollision) || c.TimeToCollision < earliestCollision.TimeToCollision) {
          earliestCollision = c;
          *ppb1 = pBalli;
          *ppb2 = pBallj;
        }
      }
      pBallj = pBallj->pNext;
    }
    pBalli = pBalli->pNext;
  }
  return earliestCollision;
}

/*********************************************************************
*
*        BALLSSIM_FindEarliestCollisionWithWall
*/
static COLLISION BALLSSIM_FindEarliestCollisionWithWall(BALLSIM * pBallsim, BALL ** ppb) {
  COLLISION earliestCollision;
  COLLISION c;
  BALL    * pBalli;
  
  _COLLISION_Reset(&earliestCollision);
  //
  // If there are no walls, return no collision
  //
  if (!pBallsim->HasWalls) {
    return earliestCollision;
  }
  //
  // Check each ball to see if any collide. Store the earliest colliding ball.
  //
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    c = _COLLISION_FindTimeUntilBallCollidesWithWall(pBalli, &pBallsim->Walls);
    if (_COLLISION_Ball1HasCollisionWithWall(&c)) {
      if (!_COLLISION_Ball1HasCollision(&earliestCollision) || c.TimeToCollision < earliestCollision.TimeToCollision) {
        earliestCollision = c;
        *ppb = pBalli;
      }
    }
    pBalli = pBalli->pNext;
  }
  return earliestCollision;
}

/*********************************************************************
*
*        BALLSSIM_FindEarliestCollision
*/
static COLLISION BALLSSIM_FindEarliestCollision(BALLSIM * pBallsim, BALL ** ppb1, BALL ** ppb2) {
  COLLISION earliestCollision;
  COLLISION cWalls;
  BALL    * pbCollideWithWall;
  
  earliestCollision = BALLSSIM_FindEarliestCollisionOfTwoBalls(pBallsim, ppb1, ppb2);
  if (pBallsim->HasWalls) {
    cWalls = BALLSSIM_FindEarliestCollisionWithWall(pBallsim, &pbCollideWithWall);
    if (_COLLISION_Ball1HasCollisionWithWall(&cWalls)) {
      if (!_COLLISION_Ball1HasCollisionWithBall(&earliestCollision) || cWalls.TimeToCollision < earliestCollision.TimeToCollision) {
        earliestCollision = cWalls;
        *ppb1 = pbCollideWithWall;
      }
    }
  }
  return earliestCollision;
}

/*********************************************************************
*
*        _BALLSIM_AdvanceBallGravity
*/
static void _BALLSIM_AdvanceBallGravity(BALLSIM * pBallsim, const float dt) {
  BALL    * pBalli;
  BALL    * pBallj;
  
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    pBallj = pBalli->pNext;
    while (pBallj) {
      _BALL_DoBallGravity(pBalli, pBallj, dt, pBallsim->pConfig->Gravity);
      pBallj = pBallj->pNext;
    }
    pBalli = pBalli->pNext;
  }
}

/*********************************************************************
*
*       _BALLSIM_AdvanceGroundGravity
*/
static void _BALLSIM_AdvanceGroundGravity(BALLSIM * pBallsim, const float dt) {
  BALL * pBalli;

  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    _BALL_DoGroundGravity(pBalli, dt, pBallsim->pConfig->Gravity);
    pBalli = pBalli->pNext;
  }
}

/*********************************************************************
*
*        _BALLSSIM_AdvanceSim
*/
static void _BALLSSIM_AdvanceSim(BALLSIM * pBallsim, const float dt) {
  float     tElapsed;
  COLLISION c;
  BALL    * pb1;
  BALL    * pb2;
  unsigned  i;

  tElapsed = 0.f;
  for (i = 0; i < pBallsim->MaxCollisions; i++) {
    //
    // Find earliest collision
    //
    c = BALLSSIM_FindEarliestCollision(pBallsim, &pb1, &pb2);
    //
    // If no collisions, break
    //
    if (!_COLLISION_Ball1HasCollision(&c)) {
      break;
    }
    //
    // Is collision within the time frame?
    // Note: condition is tElapsed + timeToCollision strictly < dt, not <=, because if the two were exactly
    // equal, we would perform the velocity adjustment for collision but not move the balls any more, so the
    // collision could be detected again on the next call to advanceSim().
    //
    if (tElapsed + c.TimeToCollision < dt) {
      //
      // Collision is within time frame
      // Advance balls to point of collision
      //
      _BALLSIM_AdvanceBallPositions(pBallsim, c.TimeToCollision);
      //
      // Collision is now occuring. Do collision calculation
      //
      if (_COLLISION_Ball1HasCollisionWithWall(&c)) {
        _COLLISION_DoElasticCollisionWithWall(pb1, c.WhichWall);
      } else if (_COLLISION_Ball1HasCollisionWithBall(&c)) {
        _COLLISION_DoElasticCollisionTwoBalls(pb1, pb2);
      }
      tElapsed += c.TimeToCollision;  // Move time counter forward
    } else {
      break;  // Break if collision is not within this frame
    }
  }
  //
  // Advance ball positions further if necessary after any collisions to complete the time frame
  //
  _BALLSIM_AdvanceBallPositions(pBallsim, dt - tElapsed);
  //
  // Manage ball gravity
  //
  if (pBallsim->pConfig->HasBallGravity) {
    _BALLSIM_AdvanceBallGravity(pBallsim, dt);
  }
  //
  // Manage ground gravity
  //
  if (pBallsim->pConfig->HasGroundGravity) {
    _BALLSIM_AdvanceGroundGravity(pBallsim, dt);
  }
}

/*********************************************************************
*
*        _BALLSSIM_MoveBallToWithinBounds
*/
static void _BALLSSIM_MoveBallToWithinBounds(BALLSIM * pBallsim, BALL * pb) {
  //
  // Check wall X1
  //
  if (pb->p.x - pb->r < pBallsim->Walls.x1) {
    _VECTOR_SetX(&pb->p, pBallsim->Walls.x1 + pb->r);
  }
  //
  // Check wall Y1
  //
  if (pb->p.y - pb->r < pBallsim->Walls.y1) {
    _VECTOR_SetY(&pb->p, pBallsim->Walls.y1 + pb->r);
  }
  //
  // Check wall X2
  //
  if (pb->p.x + pb->r > pBallsim->Walls.x2) {
    _VECTOR_SetX(&pb->p, pBallsim->Walls.x2 - pb->r);
  }
  //
  // Check wall Y2
  //
  if (pb->p.y + pb->r > pBallsim->Walls.y2) {
    _VECTOR_SetY(&pb->p, pBallsim->Walls.y2 - pb->r);
  }
}

/*********************************************************************
*
*        _BALLSSIM_MoveWalls
*/
static void _BALLSSIM_MoveWalls(BALLSIM * pBallsim, const WALLS * pNewWalls) {
  BALL * pBalli;

  pBallsim->Walls = *pNewWalls;
  pBallsim->HasWalls = 1;
  pBalli = pBallsim->pFirstBall;
  while (pBalli) {
    _BALLSSIM_MoveBallToWithinBounds(pBallsim, pBalli);
    pBalli = pBalli->pNext;
  }
}

/*********************************************************************
*
*        _BALLSSIM_AddBall
*/
static void _BALLSSIM_AddBall(BALLSIM * pBallsim, BALL * pNewBall) {
  BALL * pBalli;

  pBallsim->NumBalls = 0;
  pBalli = pBallsim->pFirstBall;
  if (pBalli) {
    pBallsim->NumBalls++;
    while (pBalli->pNext) {
      pBallsim->NumBalls++;
      pBalli = pBalli->pNext;
    }
    pBallsim->NumBalls++;
    pBalli->pNext = pNewBall;
  } else {
    pBallsim->NumBalls++;
    pBallsim->pFirstBall = pNewBall;
    pNewBall->pNext = NULL;
  }
  pBallsim->MaxCollisions = pBallsim->MaxCollisionsPerBall * pBallsim->NumBalls;
  _BALLSSIM_MoveBallToWithinBounds(pBallsim, pNewBall);
  if (pNewBall->r * 2.f > pBallsim->MaxDiameter) {
    pBallsim->MaxDiameter = pNewBall->r * 2.f;
    pBallsim->MinArea += 4.f * pNewBall->r * pNewBall->r;
  }
}

/*********************************************************************
*
*       _BALLSSIM_Delete
*/
static void _BALLSSIM_Delete(BALLSIM * pBallsim) {
  _BALLSIM_ResetBalls(pBallsim);
  _Free(pBallsim);
}

/*********************************************************************
*
*       _AdvanceSim
*/
static void _AdvanceSim(BALLSIM * pBallsim, const float dt) {
  float     tElapsed;
  COLLISION c;
  BALL    * pb1;
  BALL    * pb2;
  unsigned  i;

  tElapsed = 0.f;
  for (i = 0; i < pBallsim->MaxCollisions; i++) {
    //
    // Find earliest collision
    //
    c = BALLSSIM_FindEarliestCollision(pBallsim, &pb1, &pb2);
    //
    // If no collisions, break
    //
    if (!_COLLISION_Ball1HasCollision(&c)) {
      break;
    }
    //
    // Is collision within the time frame?
    // Note: condition is tElapsed + timeToCollision strictly < dt, not <=, because if the two were exactly
    // equal, we would perform the velocity adjustment for collision but not move the balls any more, so the
    // collision could be detected again on the next call to advanceSim().
    //
    if (tElapsed + c.TimeToCollision < dt) {
      //
      // Collision is within time frame
      // Advance balls to point of collision
      //
      _BALLSIM_AdvanceBallPositions(pBallsim, c.TimeToCollision);
      //
      // Collision is now occuring. Do collision calculation
      //
      if (_COLLISION_Ball1HasCollisionWithWall(&c)) {
        _COLLISION_DoElasticCollisionWithWall(pb1, c.WhichWall);
      } else if (_COLLISION_Ball1HasCollisionWithBall(&c)) {
        _COLLISION_DoElasticCollisionTwoBalls(pb1, pb2);
      }
      tElapsed += c.TimeToCollision;  // Move time counter forward
    } else {
      break;  // Break if collision is not within this frame
    }
  }
  //
  // Advance ball positions further if necessary after any collisions to complete the time frame
  //
  _BALLSIM_AdvanceBallPositions(pBallsim, dt - tElapsed);
  //
  // Manage ball gravity
  //
  if (pBallsim->pConfig->HasBallGravity) {
    _BALLSIM_AdvanceBallGravity(pBallsim, dt);
  }
  //
  // Manage ground gravity
  //
  if (pBallsim->pConfig->HasGroundGravity) {
    _BALLSIM_AdvanceGroundGravity(pBallsim, dt);
  }
}

/*************************** End of file ****************************/
