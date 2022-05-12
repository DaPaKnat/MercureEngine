/****************************************************************************************
 
   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.
 
   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

//! \file fbxaxissystem.h
#ifndef _FBXSDK_SCENE_AXIS_SYSTEM_H_
#define _FBXSDK_SCENE_AXIS_SYSTEM_H_

#include <fbxsdk/fbxsdk_def.h>

#include <fbxsdk/core/base/fbxarray.h>
#include <fbxsdk/core/base/fbxstring.h>

#include <fbxsdk/fbxsdk_nsbegin.h>

/** This class represents the coordinate system of the scene and can convert scenes 
    to other coordinate systems. By default the FbxScene uses a Y-Up axis
    system. If the calling application wishes to change the default axis it will need to define the new
    axis system and call the convert method with the scene as argument. The appropriate transforms will be
    applied to the first level objects of the scene only (objects whose parent is the scene itself). Child
    objects do not need to be transformed since they inherit from their parents.
    The adjustment will affect the translation animation curves and the objects pivots values (the rotation 
    transformation is applied as a pre-rotation transform therefore the rotation animation curves do not need to
    be transformed). Once converted, the scene will have its axis definition changed to the new system. 
  * \nosubgrouping
  */
class FBXSDK_DLL FbxAxisSystem
{
public:

    /** \enum EUpVector Specifies which canonical axis represents up in the system (typically Y or Z). 
      */
    enum EUpVector
	{
        eXAxis = 1,
        eYAxis = 2,
        eZAxis = 3
    };
    
    /** \enum EFrontVector  Vector with origin at the screen pointing toward the camera.
      *                     This is a subset of enum EUpVector because axis cannot be repeated.
	  *                     We use the system of "parity" to define this vector because its value (X,Y or Z axis)
	  *						really depends on the up-vector. The EPreDefinedAxisSystem list the up-vector, parity and
	  *                     coordinate system values for the predefined systems.
	  * \see Detailed description of FbxAxisSystem.
      */
    enum EFrontVector
	{
        eParityEven = 1,
        eParityOdd = 2
    };

    /** \enum ECoordSystem Specifies the third vector of the system.
	  *                   The FbxAxisSystem deduces the correct vector and direction based on this flag
	  *                   and the relationship with the up and front vectors. The EPreDefinedAxisSystem list the up-vector, parity and
	  *                   coordinate system values for the predefined systems.
      */
    enum ECoordSystem
	{
        eRightHanded,
        eLeftHanded
    };

    /** \enum EPreDefinedAxisSystem  Enumeration that can be used to initialize a new instance of this class with
	  *                              predefined configurations (see the "Predefined axis systems" section).
      */
    enum EPreDefinedAxisSystem
	{
        eMayaZUp,			/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
        eMayaYUp,			/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
        eMax,				/*!< UpVector = ZAxis, FrontVector = -ParityOdd, CoordSystem = RightHanded */
        eMotionBuilder,		/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
        eOpenGL,			/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = RightHanded */
        eDirectX,			/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
        eLightwave			/*!< UpVector = YAxis, FrontVector =  ParityOdd, CoordSystem = LeftHanded */
    };

    /** 
      * \name Constructor and Destructor
      */
    //@{
		FbxAxisSystem();

		/** Constructor!
		  * \param pUpVector Specify the up vector.
		  * \param pFrontVector Specify the front vector.
		  * \param pCoorSystem Specify RightHanded coordinate system or LeftHanded coordinate system.
		  */
		FbxAxisSystem(EUpVector pUpVector, EFrontVector pFrontVector, ECoordSystem pCoorSystem);

		/** Copy constructor!
		  * \param pAxisSystem Another FbxAxisSystem object copied to this one.
		  */
		FbxAxisSystem(const FbxAxisSystem& pAxisSystem);

		/** Constructor!
		  * \param pAxisSystem Specify which predefined axis system to copy.
		  */
		FbxAxisSystem(const EPreDefinedAxisSystem pAxisSystem);

		//! Destructor.
		virtual ~FbxAxisSystem();
    //@}

    /**
      * \name Boolean operation.
      */
    //@{

    /** Equivalence operator.
      * \param pAxisSystem The axis system to compare against this one.
      * \return \c true if these two axis systems are equal, \c false otherwise.
      */
    bool operator==(const FbxAxisSystem& pAxisSystem)const;

    /** Non-equivalence operator.
      * \param pAxisSystem The axis system to compare against this one.
      * \return \c true if these two axis systems are unequal, \c false otherwise.
      */
    bool operator!=(const FbxAxisSystem& pAxisSystem)const;
    //@}

    /** Assignment operation.
      * \param pAxisSystem Axis system assigned to this one.
      */
    FbxAxisSystem& operator=(const FbxAxisSystem& pAxisSystem);

	/**
      * \name Predefined axis systems.
	  *	These static members define the axis system of the most popular applications.
      */
    //@{

    //! Predefined axis system: MayaZUp (UpVector = +Z, FrontVector = -Y, CoordSystem = +X (RightHanded))
    static const FbxAxisSystem MayaZUp;

    //! Predefined axis system: MayaYUp (UpVector = +Y, FrontVector = +Z, CoordSystem = +X (RightHanded))
    static const FbxAxisSystem MayaYUp;

    //! Predefined axis system: Max (UpVector = +Z, FrontVector = -Y, CoordSystem = +X (RightHanded))
    static const FbxAxisSystem Max;

    //! Predefined axis system: Motionbuilder (UpVector = +Y, FrontVector = +Z, CoordSystem = +X (RightHanded))
    static const FbxAxisSystem Motionbuilder;

    //! Predefined axis system: OpenGL (UpVector = +Y, FrontVector = +Z, CoordSystem = +X (RightHanded))
    static const FbxAxisSystem OpenGL;

    //! Predefined axis system: DirectX (UpVector = +Y, FrontVector = +Z, CoordSystem = -X (LeftHanded))
    static const FbxAxisSystem DirectX;

    //! Predefined axis system: Lightwave (UpVector = +Y, FrontVector = +Z, CoordSystem = -X (LeftHanded))
    static const FbxAxisSystem Lightwave;
	//@}

    /** Convert a scene to this axis system. Sets the axis system of the scene to this system unit. 
      * \param pScene     The scene to convert
      */
    void ConvertScene(FbxScene* pScene) const;

    /** Convert a scene to this axis system by using the specified
      *             node as an Fbx_Root. This is provided for backwards compatibility
      *             only and ConvertScene(FbxScene* pScene) should be used instead when possible.
      * \param pScene       The scene to convert
      * \param pFbxRoot     The Fbx_Root node that will be transformed.
      */
    void ConvertScene(FbxScene* pScene, FbxNode* pFbxRoot) const;
	
    /** Get the EFrontVector and its sign of this axis system.
      * \param pSign The sign of the axis, 1 for front, -1 for back (relative to observer).
      * \return The EFrontVector of this axis system.
      */
    EFrontVector GetFrontVector( int & pSign ) const;

    /** Get the EUpVector and its sign of this axis system.
      * \param pSign The sign of the axis, 1 for up, -1 for down (relative to observer).
      * \return The EUpVector of this axis system.
      */
    EUpVector GetUpVector( int & pSign ) const;

    /** Accessor to the ECoordSystem of this object.
      * \return The current coordinate axis system of this object.
      */
    ECoordSystem GetCoorSystem() const;

	/** Represents the axis system as a 4x4 matrix
	  * \return The equivalent matrix of this axis system
	  */
	void GetMatrix(FbxAMatrix& pMatrix);

	/** Converts the children of the given node to this axis system.
      *             Unlike the ConvertScene() method, this method does not set the axis system 
      *             of the scene that the pRoot node belongs, nor does it adjust FbxPose
      *             as they are not stored under the scene, and not under a particular node.
      * \param pRoot The node whose children are converted.
      * \param pSrcSystem The source axis system.
      */
    void ConvertChildren(FbxNode* pRoot, const FbxAxisSystem& pSrcSystem) const;

/*****************************************************************************************************************************
** WARNING! Anything beyond these lines is for internal use, may not be documented and is subject to change without notice! **
*****************************************************************************************************************************/
#ifndef DOXYGEN_SHOULD_SKIP_THIS
protected:
	class AxisDef
	{
	public:
		enum EAxis {eXAxis, eYAxis, eZAxis};
		bool operator==(const AxisDef& pAxis) const { return (mAxis == pAxis.mAxis) && (mSign == pAxis.mSign); }
		EAxis	mAxis;
		int		mSign;
	};

    AxisDef mUpVector;
    AxisDef mFrontVector;
    AxisDef mCoorSystem;

    void ConvertTProperty(FbxArray<FbxNode*>& pNodes, const FbxAxisSystem& pFrom) const;
    void ConvertCurveNodes(FbxArray<FbxAnimCurveNode*>& pCurveNodes, const FbxAxisSystem& pFrom) const;
    void AdjustPreRotation(FbxNode* pNode, const FbxMatrix& pConversionRM) const;
    void AdjustPivots(FbxNode* pNode, const FbxMatrix& pConversionRM) const;
    void GetConversionMatrix(const FbxAxisSystem& pFrom, FbxMatrix& pConversionRM) const;
    void AdjustLimits(FbxNode* pNode, const FbxMatrix& pConversionRM) const;
    void AdjustPoses(FbxScene* pScene, const FbxMatrix& pConversionRM) const;
    void AdjustCamera(FbxNode* pNode, const FbxMatrix& pConversionRM ) const;
    void AdjustCluster(FbxNode* pNode, const FbxMatrix& pConversionRM) const;
    void ConvertChildren(FbxNode* pRoot, const FbxAxisSystem& pSrcSystem, bool pSubChildrenOnly) const;

    friend class FbxGlobalSettings;
#endif /* !DOXYGEN_SHOULD_SKIP_THIS *****************************************************************************************/
};

#include <fbxsdk/fbxsdk_nsend.h>

#endif /* _FBXSDK_SCENE_AXIS_SYSTEM_H_ */
