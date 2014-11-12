{-# LANGUAGE DeriveFunctor #-}
{-# LANGUAGE DeriveFoldable #-}
{-# LANGUAGE DeriveTraversable #-}
--
-- Flight/Param.hs --- SMACCMPilot flight parameters.
--
-- Copyright (C) 2013, Galois, Inc.
-- All Rights Reserved.
--

module SMACCMPilot.Flight.Param where

import Control.Applicative ((<*>), (<$>))
import Data.Foldable (Foldable)
import Data.Traversable (Traversable)

import SMACCMPilot.Param (Param, ParamT, param, group)

-- | PID controller parameters.
data PIDParams f = PIDParams
  { pidP    :: Param f
  , pidI    :: Param f
  , pidD    :: Param f
  , pidImax :: Param f
  } deriving (Functor, Foldable, Traversable)

-- | Initialize PID parameters with default values.
pidParams :: Monad m => Float -> Float -> Float -> Float
          -> ParamT f m (PIDParams f)
pidParams p i d imax =
  PIDParams <$> param "P" p
            <*> param "I" i
            <*> param "D" d
            <*> param "IMAX" imax

data StabilizerParams f = StabilizerParams
  { stabPosition :: PIDParams f
  , stabRate     :: PIDParams f
  } deriving (Functor, Foldable, Traversable)

-- | Initialize PID parameters with default values.
stabParams :: Monad m
           => Float -> Float -> Float -> Float
           -> Float -> Float -> Float -> Float
           -> ParamT f m (StabilizerParams f)
stabParams pos_p pos_i pos_d pos_imax rate_p rate_i rate_d rate_imax =
  StabilizerParams <$> group "STAB" (pidParams pos_p pos_i pos_d pos_imax)
                   <*> group "RATE" (pidParams rate_p rate_i rate_d rate_imax)

-- | Altitude controller parameters.
data AltitudeParams f = AltitudeParams
  { altitudeRateThrust :: PIDParams f
  , altitudePosition   :: PIDParams f
  , altitudeUI         :: ThrUIParams f
  } deriving (Functor, Foldable, Traversable)

-- | Position controller parameters.
data PosCtlParams f = PosCtlParams
  { posCtlThrust :: PIDParams f
  , posUISens    :: Param f
  } deriving (Functor, Foldable, Traversable)

-- | PID controller parameters.
data ThrUIParams  f = ThrUIParams 
  { thrUIsens :: Param f  -- Meters per second at full scale
  , thrUIdead :: Param f  -- Deadband as a fraction of full scale (0..1)
  } deriving (Functor, Foldable, Traversable)

thrUIParams :: Monad m => Float -> Float
            -> ParamT f m (ThrUIParams f)
thrUIParams s d =
  ThrUIParams <$> param "SENS" s
              <*> param "DEAD" d

-- | Flight control parameters.
data FlightParams f = FlightParams
  { flightRoll      :: StabilizerParams f
  , flightPitch     :: StabilizerParams f
  , flightPRUISens  :: Param f
  , flightYaw       :: StabilizerParams f
  , flightYawUISens :: Param f
  , flightAltitude  :: AltitudeParams f
  , flightPosition  :: PosCtlParams f
  , flightFSAlt     :: Param f
  } deriving (Functor, Foldable, Traversable)

-- | Initialize flight parameters to their default values.
flightParams :: Monad m => ParamT f m (FlightParams f)
flightParams =                          -- P     I     D     IMAX (-IMIN)
  FlightParams <$> group "RLL" (stabParams 3.500 0.000 0.000 8.0
                                           0.045 0.000 0.0005 0.25)
               <*> group "PIT" (stabParams 2.500 0.000 0.000 8.0
                                           0.045 0.000 0.0005 0.25)
               <*> param "PITRLL_UI_SENS"  45.0
               <*> group "YAW" (stabParams 1.0   0.000 0.000 8.0
                                           0.305 0.015 0.0   1.0)
               <*> param "YAW_UI_SENS"     180.0
               <*> group "ALT" altitudeParams
               <*> group "POS" posCtlParams
               <*> param "FS_ALT_OFFS"     2.0
  where
  altitudeParams :: Monad m => ParamT f m (AltitudeParams f)
  altitudeParams =                              -- P     I     D     IMAX
    AltitudeParams <$> group "RATE" (pidParams 0.050 0.000 0.007 0.8)
                   <*> group "POS"  (pidParams 0.600 0.000 0.000 5.0)
                                             -- sens deadband
                   <*> group "UI"   (thrUIParams 1.0 0.30)

  posCtlParams :: Monad m => ParamT f m (PosCtlParams f)
  posCtlParams =                            -- P     I     D     IMAX
    PosCtlParams <$> group "THRUST" (pidParams 0.200 0.010 0.010 0.4)
                 <*> param "UI_SENS" 2.0

