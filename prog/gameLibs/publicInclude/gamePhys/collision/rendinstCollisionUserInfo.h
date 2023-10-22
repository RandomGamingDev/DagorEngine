//
// Dagor Engine 6.5 - Game Libraries
// Copyright (C) 2023  Gaijin Games KFT.  All rights reserved
// (for conditions of use see prog/license.txt)
//
#pragma once

#include <phys/dag_physUserData.h>

#include <rendInst/rendInstCollision.h>
#include <gamePhys/collision/cachedCollisionObject.h>

struct RendinstCollisionUserInfo : public PhysObjectUserData
{
  struct RendinstImpulseThresholdData : public CachedCollisionObjectInfo
  {
    rendinst::CollisionInfo collInfo;

    RendinstImpulseThresholdData(float impulse, const rendinst::RendInstDesc &ri_desc, float at_time,
      const rendinst::CollisionInfo &coll_info);

    virtual ~RendinstImpulseThresholdData();

    virtual float onImpulse(float impulse, const Point3 &dir, const Point3 &pos, float point_vel, int32_t user_data = -1,
      gamephys::ImpulseLogFunc /*log_func*/ = nullptr) override;
    virtual float getDestructionImpulse() const override;
    virtual bool isRICollision() const override;
  };

  struct TreeRendinstImpulseThresholdData : public CachedCollisionObjectInfo
  {
    Point3 finalImpulse;
    TMatrix invRiTm;
    float lastPointVel;
    float lastOmega;
    rendinst::CollisionInfo collInfo;

    TreeRendinstImpulseThresholdData(float impulse, const rendinst::RendInstDesc &ri_desc, float at_time,
      const rendinst::CollisionInfo &coll_info);

    virtual ~TreeRendinstImpulseThresholdData();

    virtual float onImpulse(float impulse, const Point3 &dir, const Point3 &pos, float point_vel, int32_t user_data = -1,
      gamephys::ImpulseLogFunc /*log_func*/ = nullptr) override;
    virtual float getDestructionImpulse() const override;
    virtual bool isTreeCollision() const override;
  };

  rendinst::RendInstDesc desc;
  BBox3 bbox;
  TMatrix tm;
  bool isTree;
  bool immortal;
  bool isDestr;
  bool bushBehaviour;
  bool treeBehaviour;
  gamephys::CollisionObjectInfo *objInfoData;
  mutable bool collided;

  RendinstCollisionUserInfo(const rendinst::RendInstDesc &from_desc);
};
