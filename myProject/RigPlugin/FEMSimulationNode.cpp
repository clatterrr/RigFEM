#include "StdAfx.h"
#include "FEMSimulationNode.h"

using namespace RigFEM; 
 
MTypeId RigSimulationNode::m_id(NODE_FEM_SIMULATION_ID); 
const char* RigSimulationNode::m_nodeName = NODE_FEM_SIMULATION_NAME;

const char* RigSimulationNode::m_initParamName[2] = {"rigInitParameter","rInitParam"};
const char* RigSimulationNode::m_paramName[2] = {"rigParameter","rParam"};
const char* RigSimulationNode::m_meshName[2] = {"riggedMesh", "rMesh"};
const char* RigSimulationNode::m_transformMatrixName[2] = {"meshTransform", "trans"};
const char*	RigSimulationNode::m_tetEdgeRatioName[2]={"tetEdgeRatio", "edgeRatio"};			// 体网格化参数，四面体边比例
const char*	RigSimulationNode::m_tetMaxVolumeName[2]={"tetMaxVolume", "maxVolume"};			// 体网格化参数，四面体最大体积
const char*	RigSimulationNode::m_youngModulusName[2]={"youngModulus", "yModulus"};			// 杨氏模量
const char*	RigSimulationNode::m_nuName[2] = {"nu","nu"};					// 控制不同轴向变形影响程度的参数
const char*	RigSimulationNode::m_densityName[2]={"density","den"};				// 密度
const char*	RigSimulationNode::m_timeStepName[2]={"stepTime","dt"};				// 时间步长
const char* RigSimulationNode::m_deriStepName[2]={"derivativeStep", "dStep"};   // 导数步长
const char* RigSimulationNode::m_minGradSizeName[2] = {"inverseGradTolerance", "invGradSize"};
const char* RigSimulationNode::m_minStepSizeName[2] = {"inverseStepTolerance", "invStepSize"};
const char* RigSimulationNode::m_maxIterName[2] = {"maxIteration", "maxIter"};
const char* RigSimulationNode::m_simTypeName[2] = {"simulationType", "simType"};
const char* RigSimulationNode::m_weightPathName[2]={"weightPath","wPath"};
const char* RigSimulationNode::m_maxParamStepName[2] = {"maxParamStepEachIteration", "maxParamStep"};
const char* RigSimulationNode::m_dispTypeName[2] = {"displayType", "dispType"};
const char* RigSimulationNode::m_dispFemMeshName[2] = {"displayFemMesh","dispFem"};
const char* RigSimulationNode::m_cgMinStepSizeName[2] = {"inverseStepToleranceOfCG","invCGStepSize"};
const char* RigSimulationNode::m_maxCGIterName[2]={"maxIterationOfCG","maxCGIter"};
const char* RigSimulationNode::m_inputForceName[2]={"inputForce","ifc"};
const char* RigSimulationNode::m_fieldDataName[2]={"fieldData", "fd"};
const char* RigSimulationNode::m_fieldDataDeltaTimeName[2]={"fieldDataDeltaTime", "fdt"};
const char* RigSimulationNode::m_fieldDataMassName[2] = {"fieldDataMass", "fdm"};
const char* RigSimulationNode::m_fieldDataVelocityName[2] = {"fieldDataVelocity", "fdv"};
const char* RigSimulationNode::m_fieldDataPositionName[2]={"fieldDataPosition", "fdp"};
const char* RigSimulationNode::m_fieldForceFactorName[2]={"fieldForceShowLength","fieldForceLen"};
const char* RigSimulationNode::m_derivativeGainRatioName[2] =  {"derivativeGainRatio", "dGainRatio"};
const char* RigSimulationNode::m_proportionalGainName[2] = {"proportionalGain", "pGain"};
const char* RigSimulationNode::m_targetParamName[2]= {"targetParam", "tarParam"};
const char* RigSimulationNode::m_controlTypeName[2]= {"controlType", "ctrlType"};
const char* RigSimulationNode::m_resultPathName[2]={"resultPath","resPath"};

MObject RigSimulationNode::m_resultPath;
MObject RigSimulationNode::m_derivativeGainRatio;
MObject RigSimulationNode::m_proportionalGain;
MObject RigSimulationNode::m_targetParam;
MObject RigSimulationNode::m_controlType;
MObject RigSimulationNode::m_fieldForceFactor;
MObject RigSimulationNode::m_fieldDataPosition;	// 位置子属性
MObject RigSimulationNode::m_fieldDataVelocity;	// 速度子属性
MObject RigSimulationNode::m_fieldDataMass;		// 质量子属性
MObject RigSimulationNode::m_fieldDataDeltaTime;	// 时间子属性
MObject RigSimulationNode::m_fieldData;
MObject RigSimulationNode::m_inputForce;
MObject	RigSimulationNode::m_maxCGIter;
MObject RigSimulationNode::m_dispFemMesh;
MObject RigSimulationNode::m_cgMinStepSize;
MObject RigSimulationNode::m_dispType;
MObject RigSimulationNode::m_maxParamStep;
MObject RigSimulationNode::m_weightPath;
MObject RigSimulationNode::m_simType;
MObject RigSimulationNode::m_minGradSize;
MObject RigSimulationNode::m_minStepSize;
MObject RigSimulationNode::m_maxIter; 
MObject RigSimulationNode::m_deriStep;
MObject RigSimulationNode::m_transformMatrix;
MObject RigSimulationNode::m_param;   
MObject	RigSimulationNode::m_initParam;
MObject RigSimulationNode::m_mesh;
MObject	RigSimulationNode::m_tetEdgeRatio;			// 体网格化参数，四面体边比例
MObject	RigSimulationNode::m_tetMaxVolume;			// 体网格化参数，四面体最大体积
MObject	RigSimulationNode::m_youngModulus;			// 杨氏模量
MObject	RigSimulationNode::m_nu;					// 控制不同轴向变形相互影响的参数
MObject	RigSimulationNode::m_density;				// 密度
MObject	RigSimulationNode::m_timeStep;				// 时间步长

RigSimulationNode::RigSimulationNode(void):
m_box(MPoint(-1.1,-0.5,-1.1), MPoint(4.1,0.5,1.1)),
m_simTypeFlag(RigSimulationNode::SIM_STANDARD),
m_simulator(NULL)
{
}

RigSimulationNode::~RigSimulationNode(void)
{
	clearRig();
}

void RigSimulationNode::postConstructor()
{
	MStatus s;
	MFnDependencyNode nodeFn(thisMObject(), &s);
	if (s)
	{
		nodeFn.setName( "rigSimulationShape#", &s);
	}
}

void RigSimulationNode::draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView:: DisplayStatus )
{
	
	view.beginGL();
	glPushAttrib(GL_CURRENT_BIT);

	drawIcon();

	//testField();
	//printFieldData();

	m_box = MBoundingBox(MPoint(-1.1,-0.5,-1.1), MPoint(4.1,0.5,1.1));
	if (m_simulator)
	{
		int curFrame = getCurFrame();

		MMatrix mat  = path.inclusiveMatrixInverse();
		double  matBuf[4][4];
		mat.get(matBuf);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glMultMatrixd(&matBuf[0][0]);

		// 更新参数值
		EigVec p;
		if(m_simulator->getParam(curFrame, p) && getDispType() == DISP_SIM)
			setParamPlug(&p[0], p.size());
		else
			setParamToInit();
		double bbox[6];

		MPlug dispFemMeshPlug = Global::getPlug(this, m_dispFemMeshName[0]);
		bool isDrawFemMesh = (bool)dispFemMeshPlug.asShort();
		if (isDrawFemMesh)
		{
			MPlug fieldForceFactorPlug = Global::getPlug(this, m_fieldForceFactorName[0]);
			m_simulator->setExternalForceDispFactor(fieldForceFactorPlug.asDouble());
			bool res = m_simulator->showStatus(curFrame, bbox);
			if (res)
			{
				// 更新包围盒
				double xformBox[6];
				Utilities::transformBBox(bbox, bbox+3, matBuf, xformBox, xformBox+3);
				m_box.expand(MPoint(xformBox[0], xformBox[1], xformBox[2]));
				m_box.expand(MPoint(xformBox[3], xformBox[4], xformBox[5]));	 
			}
		}

		glPopMatrix();
	}
	glPopAttrib();
	view.endGL();
}

MBoundingBox RigSimulationNode::boundingBox() const
{
	return m_box;
}

MStatus RigSimulationNode::initialize()
{
	MStatus s;
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute  mAttr;
	MFnTypedAttribute   tAttr;
	MFnEnumAttribute	eAttr;
	MFnUnitAttribute	uAttr;
	MFnCompoundAttribute cAttr;

	m_param = nAttr.create(m_paramName[0], m_paramName[1], MFnNumericData::kDouble,0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(false); 
	nAttr.setReadable(true);
	nAttr.setSoftMin(0);
	nAttr.setSoftMax(1);
	nAttr.setArray(true); 
	nAttr.setUsesArrayDataBuilder(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_initParam = nAttr.create(m_initParamName[0], m_initParamName[1], MFnNumericData::kDouble,0, &s);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);
	nAttr.setArray(true); 
	nAttr.setUsesArrayDataBuilder(true);
	nAttr.setAffectsAppearance(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_tetEdgeRatio = nAttr.create(m_tetEdgeRatioName[0], m_tetEdgeRatioName[1], MFnNumericData::kDouble,2.0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(1.01);
	nAttr.setMax(10);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_tetMaxVolume = nAttr.create(m_tetMaxVolumeName[0], m_tetMaxVolumeName[1], MFnNumericData::kDouble,5.0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.01);
	nAttr.setSoftMax(10);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_youngModulus = nAttr.create(m_youngModulusName[0], m_youngModulusName[1], MFnNumericData::kDouble,1e6, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.01);
	nAttr.setSoftMin(1e5);
	nAttr.setSoftMax(1e6);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_nu = nAttr.create(m_nuName[0], m_nuName[1], MFnNumericData::kDouble,0.45, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.01);
	nAttr.setMax(0.5);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_density = nAttr.create(m_densityName[0], m_densityName[1], MFnNumericData::kDouble,1000, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.01);
	nAttr.setSoftMin(500);
	nAttr.setSoftMax(5000);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_timeStep = nAttr.create(m_timeStepName[0], m_timeStepName[1], MFnNumericData::kDouble,1/24.0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.001);
	nAttr.setSoftMin(0.01);
	nAttr.setSoftMax(1);
	CHECK_MSTATUS_AND_RETURN_IT(s);


	m_deriStep = nAttr.create(m_deriStepName[0], m_deriStepName[1], MFnNumericData::kDouble,1e-3, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(1e-5);
	nAttr.setMax(1);
	nAttr.setSoftMin(1e-4);
	nAttr.setSoftMax(1e-2);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_minGradSize = nAttr.create(m_minGradSizeName[0], m_minGradSizeName[1], MFnNumericData::kDouble,100, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.001);
	nAttr.setMax(1e15);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_minStepSize = nAttr.create(m_minStepSizeName[0], m_minStepSizeName[1], MFnNumericData::kDouble,100, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.001);
	nAttr.setMax(1e7);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_maxIter = nAttr.create(m_maxIterName[0], m_maxIterName[1], MFnNumericData::kInt,10, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0);
	nAttr.setMax(50);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_mesh = tAttr.create(m_meshName[0], m_meshName[1], MFnData::kMesh, &s);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(false);
	tAttr.setArray(false);
	tAttr.setKeyable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_transformMatrix = mAttr.create(m_transformMatrixName[0], m_transformMatrixName[1]);
	mAttr.setHidden(false);
	mAttr.setReadable(false);
	mAttr.setWritable(true);
	mAttr.setKeyable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_simType = eAttr.create(m_simTypeName[0], m_simTypeName[1]);
	eAttr.addField("Standard", RigSimulationNode::SIM_STANDARD);
	eAttr.addField("Skin",   RigSimulationNode::SIM_SKIN);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);
	eAttr.setStorable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_dispType = eAttr.create(m_dispTypeName[0], m_dispTypeName[1]);
	eAttr.addField("Simulation", DISP_SIM);
	eAttr.addField("Initial Value", DISP_INIT);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);
	eAttr.setStorable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_weightPath = tAttr.create(m_weightPathName[0], m_weightPathName[1], MFnData::kString);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setUsedAsFilename(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);


	m_resultPath = tAttr.create(m_resultPathName[0], m_resultPathName[1], MFnData::kString);
	tAttr.setKeyable(true);
	tAttr.setStorable(true);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setUsedAsFilename(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_maxParamStep = nAttr.create(m_maxParamStepName[0], m_maxParamStepName[1], MFnNumericData::kDouble, 1, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.001);
	nAttr.setMax(1e7);
	nAttr.setSoftMin(0.1);
	nAttr.setSoftMax(3);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_dispFemMesh = nAttr.create(m_dispFemMeshName[0], m_dispFemMeshName[1], MFnNumericData::kBoolean, 1, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setAffectsAppearance(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_maxCGIter = nAttr.create(m_maxCGIterName[0], m_maxCGIterName[1], MFnNumericData::kInt,10, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0);
	nAttr.setMax(50);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_cgMinStepSize = nAttr.create(m_cgMinStepSizeName[0], m_cgMinStepSizeName[1], MFnNumericData::kDouble,10, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0.001);
	nAttr.setMax(1e7);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_inputForce = tAttr.create(m_inputForceName[0], m_inputForceName[1], MFnData::kVectorArray, &s);
	tAttr.setKeyable(false);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	tAttr.setArray(true);
	tAttr.setUsesArrayDataBuilder(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	// fieldData 属性，由多个子属性组成
	m_fieldDataPosition = tAttr.create(m_fieldDataPositionName[0], m_fieldDataPositionName[1], MFnData::kVectorArray, &s);
	tAttr.setKeyable(false);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_fieldDataVelocity = tAttr.create(m_fieldDataVelocityName[0], m_fieldDataVelocityName[1], MFnData::kVectorArray, &s);
	tAttr.setKeyable(false);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_fieldDataMass = tAttr.create(m_fieldDataMassName[0], m_fieldDataMassName[1], MFnData::kDoubleArray, &s);
	tAttr.setKeyable(false);
	tAttr.setStorable(false);
	tAttr.setHidden(false);
	tAttr.setWritable(true);
	tAttr.setReadable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_fieldDataDeltaTime = uAttr.create(m_fieldDataDeltaTimeName[0], m_fieldDataDeltaTimeName[1], MFnUnitAttribute::kTime,1, &s);
	uAttr.setKeyable(true);
	uAttr.setStorable(false);
	uAttr.setHidden(false);
	uAttr.setWritable(true);
	uAttr.setReadable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_fieldData = cAttr.create(m_fieldDataName[0], m_fieldDataName[1], &s);
	cAttr.addChild(m_fieldDataPosition);
	cAttr.addChild(m_fieldDataVelocity);
	cAttr.addChild(m_fieldDataMass);
	cAttr.addChild(m_fieldDataDeltaTime);	
	cAttr.setKeyable(false);
	cAttr.setStorable(false);
	cAttr.setHidden(false);
	cAttr.setWritable(true);
	cAttr.setReadable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_fieldForceFactor = nAttr.create(m_fieldForceFactorName[0], m_fieldForceFactorName[1], MFnNumericData::kDouble,0.0, &s);
	nAttr.setKeyable(false);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setMin(0);
	nAttr.setMax(1e10);
	nAttr.setAffectsAppearance(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	// PD控制器属性
	m_controlType = eAttr.create(m_controlTypeName[0], m_controlTypeName[1]);
	eAttr.addField("None", CONTROL_NONE);
	eAttr.addField("Implicit Force",   CONTROL_IMPLICIT_FORCE);
	eAttr.addField("Explicit Force",   CONTROL_EXPLICIT_FORCE);
	eAttr.setHidden(false);
	eAttr.setReadable(true);
	eAttr.setWritable(true);
	eAttr.setStorable(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_targetParam = nAttr.create(m_targetParamName[0], m_targetParamName[1], MFnNumericData::kDouble,0, &s);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);
	nAttr.setArray(true); 
	nAttr.setUsesArrayDataBuilder(true);
	nAttr.setAffectsAppearance(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_proportionalGain = nAttr.create(m_proportionalGainName[0], m_proportionalGainName[1], MFnNumericData::kDouble,0, &s);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);
	nAttr.setArray(true); 
	nAttr.setUsesArrayDataBuilder(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	m_derivativeGainRatio = nAttr.create(m_derivativeGainRatioName[0], m_derivativeGainRatioName[1], MFnNumericData::kDouble,0, &s);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);
	nAttr.setHidden(false);
	nAttr.setWritable(true); 
	nAttr.setReadable(true);
	nAttr.setSoftMin(-10);
	nAttr.setSoftMax(10);
	nAttr.setArray(true); 
	nAttr.setUsesArrayDataBuilder(true);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	s = addAttribute(m_param);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_initParam);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_mesh);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_transformMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_tetEdgeRatio);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_tetMaxVolume);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_youngModulus);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_nu);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_density);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_timeStep);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_deriStep);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_maxIter);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_minGradSize);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_minStepSize);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_simType);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_weightPath);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_resultPath);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_maxParamStep);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_dispType);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_dispFemMesh);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_maxCGIter);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_cgMinStepSize);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_inputForce);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_fieldData);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_fieldForceFactor);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_controlType);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_targetParam);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_proportionalGain);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = addAttribute(m_derivativeGainRatio);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	s = attributeAffects(m_initParam, m_param);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = attributeAffects(m_targetParam, m_param);
	CHECK_MSTATUS_AND_RETURN_IT(s);
	s = attributeAffects(m_dispType, m_param);
	CHECK_MSTATUS_AND_RETURN_IT(s);

	return s;
}

MStatus RigSimulationNode::setParamToInit()
{
	MStatus s;
	MPlug paramArrayPlug = Global::getPlug(this, m_paramName[0]);
	MPlug inParamArrayPlug= Global::getPlug(this, m_initParamName[0]);
	for (int phyIdx = 0; phyIdx < paramArrayPlug.numElements(&s); ++phyIdx)
	{
		MPlug paramPlug = paramArrayPlug.elementByPhysicalIndex(phyIdx,&s);
		CHECK_MSTATUS_AND_RETURN_IT(s);

		int logIdx = paramPlug.logicalIndex();
		MPlug inParamPlug = inParamArrayPlug.elementByLogicalIndex(logIdx, &s);
		CHECK_MSTATUS_AND_RETURN_IT(s);

		double oldVal,val = 0;
		inParamPlug.getValue(val);
		paramPlug.getValue(oldVal);
		if (oldVal != val)
			paramPlug.setValue(val);
	}
	return s;
}
MStatus RigSimulationNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus s;
	if (plug == m_param || plug.parent() == m_param)
	{
		// 设置当前状态的参数值
		int curFrame = getCurFrame();
		EigVec p;
		if(m_simulator && m_simulator->isReady() && m_simulator->getParam(curFrame, p))
		{
			Global::showVector(p, "P");
			setParamPlug(&p[0], p.size());
		}
		else
			setParamToInit();
	
	}
	data.setClean(plug);
	return s;
}

void* RigSimulationNode::creator()
{
	return new RigSimulationNode;
}

void RigSimulationNode::testRig()
{
	MStatus s;
	MPlug paramArrayPlug = Global::getPlug(this, m_paramName[0]);
	for (int ithPlug = 0; ithPlug < 10; ++ithPlug)
	{
		MPlug paramPlug = paramArrayPlug.elementByLogicalIndex(ithPlug, &s);
		int v = rand();
		paramPlug.setValue((double)v / 65535.0);
	}

	MPlug meshPlug = Global::getPlug(this, m_meshName[0]);
	MObject meshObj = meshPlug.asMObject();

	MFnMesh meshFn(meshObj);
	const int nVtx = meshFn.numVertices(&s);

	MItMeshVertex it(meshObj, &s);
	for (int ithVtx = 0; !it.isDone(&s) && ithVtx < 1; it.next(), ++ithVtx)
	{
		MPoint p = it.position(MSpace::kWorld);
		PRINT_F("%lf \t %lf \t %lf\n", p.x, p.y, p.z);
	}
} 

int RigSimulationNode::getNumParam()
{
	MStatus s;
	MPlug paramArrayPlug = Global::getPlug(this, m_paramName[0]);
	int nPlugs = paramArrayPlug.numElements(&s);
	for (int logIdx = 0; logIdx < nPlugs; ++logIdx)
	{
		MPlug paramPlug = paramArrayPlug.elementByLogicalIndex(logIdx,&s);
		if (!s || !paramPlug.isConnected(&s)) 
			return logIdx;
	}
	return nPlugs;
}

void RigSimulationNode::clearRig()
{
	delete m_simulator;
	m_simulator = NULL;
}

bool RigSimulationNode::resetRig()
{
	clearRig();

	int nValidParam = getNumParam();
	if (nValidParam <= 0)
	{
		PRINT_F("no param");
		return false;
	}

	// 获取网格对象
	MStatus s;
	setParamToInit();			// 设置参数值为初始参数指定的值
	MPlug meshPlug = Global::getPlug(this, m_meshName[0]);
	MObject meshObj = meshPlug.asMObject(MDGContext::fsNormal, &s);
	if (s != MS::kSuccess)
	{
		PRINT_F("failed to get mesh obj.");
		return false;
	}
	MFnMesh meshFn(meshObj);
	int nSurfVtx = meshFn.numVertices();
	if (nSurfVtx <= 0 || meshFn.numPolygons() <= 0)
	{
		PRINT_F("mesh is empty.");
		return false;
	}

	// 初始化RiggedMesh对象
	MPlug tetEdgeRatioPlug = Global::getPlug(this, m_tetEdgeRatioName[0]);
	MPlug tetMaxVolumePlug = Global::getPlug(this, m_tetMaxVolumeName[0]);
	MPlug youngModulusPlug = Global::getPlug(this, m_youngModulusName[0]);
	MPlug nuPlug = Global::getPlug(this, m_nuName[0]);
	MPlug densityPlug = Global::getPlug(this, m_densityName[0]);
	MPlug stepTimePlug = Global::getPlug(this, m_timeStepName[0]);
	MPlug weightPathPlug = Global::getPlug(this, m_weightPathName[0]);
	MPlug controlTypePlug= Global::getPlug(this, m_controlTypeName[0]);

	tetgenio tetMesh;
	MMatrix mat = getMatrix();
	bool res = MS::kSuccess == Global::maya2TetgenMesh(meshObj, tetMesh, mat);
	m_simTypeFlag = (SimulationType)Global::getPlug(this, m_simTypeName[0]).asShort();
	MString weightStr = weightPathPlug.asString();
	EigVec initParam;
	int curFrame = getCurFrame();
	getInitParam(initParam);
	
	// 初始化控制参数
	RigControlType ctrlType = (RigControlType)controlTypePlug.asShort();
	if (ctrlType != CONTROL_NONE)
	{
		EigVec tarDummy, pGDummy, dGDummy;
		getControlParams(tarDummy, pGDummy, dGDummy);
	}
	if (res)
	{
		switch(m_simTypeFlag)
		{
		case RigSimulationNode::SIM_STANDARD:
			{
				RigFEM::RigSimulator* sim = new RigFEM::RigSimulator();
				res &= sim->init(
					tetMesh, this,
					initParam,
					curFrame,
					tetMaxVolumePlug.asDouble(), 
					tetEdgeRatioPlug.asDouble(), 
					youngModulusPlug.asDouble(), 
					nuPlug.asDouble(), 
					densityPlug.asDouble(),
					stepTimePlug.asDouble()
					);
				m_simulator = sim;
			}
			break;
		case RigSimulationNode::SIM_SKIN:
			{
				RigFEM::RigSkinSimulator* sim = new RigFEM::RigSkinSimulator();
				res &= sim->init(
					tetMesh, this,
					initParam,
					weightStr.asChar(),
					curFrame,
					tetMaxVolumePlug.asDouble(), 
					tetEdgeRatioPlug.asDouble(), 
					youngModulusPlug.asDouble(), 
					nuPlug.asDouble(), 
					densityPlug.asDouble(),
					stepTimePlug.asDouble()
					);
				m_simulator = sim;
			}
			break;
		}
	}

	if (!res)
	{
		PRINT_F("failed to initialize.");
		clearRig();
		return false;
	}

	// 初始化rig对象
	updateDeriStepSize();
	return true;
}

MMatrix RigSimulationNode::getMatrix()
{
	// 获取变换矩阵
	MPlug matrixPlug  = Global::getPlug(this, RigSimulationNode::transformLongName());
	MObject matrixObj = matrixPlug.asMObject();
	MFnMatrixData matrixData(matrixObj);
	return matrixData.matrix();
}

bool RigSimulationNode::testHessian(double noiseN, double noiseP)
{
	if (!m_simulator || !m_simulator->isReady())
		return false;

	updateDeriStepSize();

	// 设置当前帧状态
	int curFrame = getCurFrame();
	return m_simulator->testHessian(curFrame);
}
bool RigSimulationNode::updateDeriStepSize()
{
	if(!m_simulator || !m_simulator->isReady())
		return false;
	MPlug deriStepPlug = Global::getPlug(this, m_deriStepName[0]);
	double deriStepVal = 1e-3;
	deriStepPlug.getValue(deriStepVal);
	m_simulator->setDeriStepSize(deriStepVal);
	return true;
}
bool RigSimulationNode::stepRig()
{
	if (!m_simulator || !m_simulator->isReady())
		return false;

	int curFrame = getCurFrame();

	// 设置上一帧状态
	updateDeriStepSize();		// 设置导数步长
	updateTerminationCond();	// 设置终止条件

	// 模拟
	return m_simulator->stepRig(curFrame);
}


int RigSimulationNode::getCurFrame()
{
	int curFrame = 1;
	MGlobal::executeCommand("currentTime -q", curFrame);
	return curFrame;
// 	return 1;
// 	MTime t = MAnimControl::currentTime();
// 	return t.value();
}

MStatus RigSimulationNode::setParamPlug( const double* param, int nParam )
{
	MStatus s;
	MPlug paramArrayPlug = Global::getPlug(this, m_paramName[0]);
	for (int ithParam = 0; ithParam < nParam; ++ithParam)
	{
		MPlug paramPlug = paramArrayPlug.elementByLogicalIndex(ithParam,&s);
		if (!s)
			break;
		
		// 参数发生更改时才更新，避免死循环
		double v;
		paramPlug.getValue(v);
		if (v != param[ithParam])
			paramPlug.setValue(param[ithParam]);
	}
	return s;
}

void RigSimulationNode::drawIcon()
{
	float p[][3] = {{0.5,0,-1},{0.8,0,1},{-1,0,1},{0.2,0,0.4}};

	glBegin(GL_LINE_LOOP);
	glVertex3fv(p[0]);
	glVertex3fv(p[1]);
	glVertex3fv(p[2]);
	glEnd();

	glBegin(GL_LINES);
	glVertex3fv(p[0]);
	glVertex3fv(p[3]);

	glVertex3fv(p[1]);
	glVertex3fv(p[3]);

	glVertex3fv(p[2]);
	glVertex3fv(p[3]); 
	glEnd();

	// draw "FEM"
	glBegin(GL_LINE_STRIP);
	glVertex3f(1.8,0,0);
	glVertex3f(1,0,0);
	glVertex3f(1,0,1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(2.8,0,0);
	glVertex3f(2,0,0);
	glVertex3f(2,0,1);
	glVertex3f(2.8,0,1);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(3,0,1);
	glVertex3f(3,0,0);
	glVertex3f(3.4,0,0.7);
	glVertex3f(3.8,0,0);
	glVertex3f(3.8,0,1);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(1,0,0.5);
	glVertex3f(1.6,0,0.5);
	glVertex3f(2,0,0.5); 
	glVertex3f(2.6,0,0.5);
	glEnd();
}

bool RigSimulationNode::updateTerminationCond()
{
	if (!m_simulator || !m_simulator->isReady())
		return false;
	if (RigSimulator* sim = dynamic_cast<RigSimulator*>(m_simulator))
	{
		MPlug maxIterPlug = Global::getPlug(this, m_maxIterName[0]);
		MPlug maxCGIterPlug = Global::getPlug(this, m_maxCGIterName[0]);
		MPlug minGradPlug = Global::getPlug(this, m_minGradSizeName[0]);
		MPlug minStepPlug = Global::getPlug(this, m_minStepSizeName[0]);
		MPlug cgMinStepPlug=Global::getPlug(this, m_cgMinStepSizeName[0]);
		MPlug maxPStepPlug= Global::getPlug(this, m_maxParamStepName[0]);
		MPlug ctrlTypePlug= Global::getPlug(this, m_controlTypeName[0]);

		int maxIter = maxIterPlug.asInt();
		int maxCGIter = maxCGIterPlug.asInt();
		double gradSize= 1.0 / minGradPlug.asDouble();
		double stepSize= 1.0 / minStepPlug.asDouble();
		double cgStepSize = 1.0 / cgMinStepPlug.asDouble();
		double maxPStep= maxPStepPlug.asDouble();
		RigControlType controlType = (RigControlType)ctrlTypePlug.asShort();
		sim->setControlType(controlType);
		NewtonSolver* solver = sim->getSolver();
		if (solver)
		{
			solver->setTerminateCond(maxIter, stepSize, gradSize, maxCGIter, cgStepSize);
			solver->setIterationMaxStepSize(maxPStep);
			solver->setControlType(controlType);
		}
	}
	return true;
}

bool RigSimulationNode::testGrad( double noiseN, double noiseP )
{
	if (!m_simulator || !m_simulator->isReady())
		return false;
	updateDeriStepSize();

	// 设置当前帧状态
	int curFrame = getCurFrame();
	return m_simulator->testGradient(curFrame);
}

bool RigSimulationNode::saveSimulationData( const char* fileName )
{
	if (!m_simulator || !m_simulator->isReady())
		return false;
	return m_simulator->saveResult(fileName);
}

bool RigSimulationNode::getInitParam( EigVec& p )
{
	MStatus s;
	int paramLength = getNumParam();

	if (paramLength <= 0)
		return false;

	p.resize(paramLength);
	MPlug paramArrayPlug = Global::getPlug(this, m_initParamName[0]);
	for (int ithParam = 0; ithParam < paramLength; ++ithParam)
	{
		MPlug paramPlug = paramArrayPlug.elementByLogicalIndex(ithParam,&s);
		if (!s)
			return false;
		paramPlug.getValue(p[ithParam]);
	}
	return true;
}

bool RigSimulationNode::staticStepRig()
{
	if (!m_simulator || !m_simulator->isReady())
		return false;

	updateDeriStepSize();

	int curFrame = getCurFrame();
	EigVec initParam;
	getInitParam(initParam);
	m_simulator->staticStepRig(curFrame, initParam);
	return true;
}

RigSimulationNode::DisplayType RigSimulationNode::getDispType()
{
	MPlug dispPlug = Global::getPlug(this, m_dispTypeName[0]);
	return (DisplayType)dispPlug.asShort();
}

MStatus RigSimulationNode::getInputForce( EigVec& force )
{
	RiggedMesh* mesh;
	MStatus s;
	if (!m_simulator || !(mesh = m_simulator->getRiggedMesh()))
		return MStatus::kFailure;

	int nDof = mesh->getNTotPnt() * 3;
	force.setZero(nDof);
	MPlug forceArrayPlug = Global::getPlug(this, m_inputForceName[0]);
	for (int phyIdx = 0; phyIdx < forceArrayPlug.numElements(&s); ++phyIdx)
	{
		MPlug forcePlug = forceArrayPlug.elementByPhysicalIndex(phyIdx, &s);
		if (!s)
			continue;

		if (!forcePlug.isConnected(&s) || !s)
			continue;

		MObject forceObj = forcePlug.asMObject();
		MFnVectorArrayData forceData(forceObj, &s);
		if (!s)
			continue;

		int vecLength = forceData.length();
		if (vecLength * 3 != nDof)
			continue;

		for (int ithVec = 0, ithDof = 0; ithVec < vecLength; ++ithVec, ithDof += 3 )
		{
			MVector v = forceData[ithVec];
			force[ithDof] += v.x;
			force[ithDof+1] += v.y;
			force[ithDof+2] += v.z;
		}
	}
	return s;
}

void RigSimulationNode::printFieldData()
{
	MPlug inputForceArrayPlug = Global::getPlug(this, m_inputForceName[0]);
	MPlug positionPlug   = Global::getPlug(this, m_fieldDataPositionName[0]);
	MPlug velocityPlug   = Global::getPlug(this, m_fieldDataVelocityName[0]);
	MPlug massPlug		 = Global::getPlug(this, m_fieldDataMassName[0]);
	MPlug dTimePlug      = Global::getPlug(this, m_fieldDataDeltaTimeName[0]);

	EigVec v;
	PRINT_F("------------------------------------");

	Global::getVectorArrayData(positionPlug, v);
	Global::showVector(v, "position");

	Global::getVectorArrayData(velocityPlug, v);
	Global::showVector(v, "velocity");

	Global::getDoubleArrayData(massPlug, v);
	Global::showVector(v, "mass");

	double dt = dTimePlug.asDouble();
	PRINT_F("deltaTime = %lf", dt);

	for (int i = 0; i < inputForceArrayPlug.numElements(); ++i)
	{
		MPlug inputForcePlug = inputForceArrayPlug.elementByPhysicalIndex(i);
		Global::getVectorArrayData(inputForcePlug, v);
		char name[20];
		sprintf(name, "inForce[%d]", inputForcePlug.logicalIndex());
		Global::showVector(v, name);
	}
}

MStatus RigSimulationNode::testField()
{
	MStatus s;

	int nPnt = 4;
	int nDof = nPnt * 3;

	MPlug posPlug = Global::getPlug(this, m_fieldDataPositionName[0]);
	MPlug velPlug = Global::getPlug(this, m_fieldDataVelocityName[0]);
	MPlug massPlug= Global::getPlug(this, m_fieldDataMassName[0]);
	MPlug dTPlug  = Global::getPlug(this, m_fieldDataDeltaTimeName[0]);

	EigVec v;
	v.setRandom(nDof);
	Global::setVectorArrayData(posPlug, v);
	v.setRandom(nDof);
	Global::setVectorArrayData(velPlug, v);
	v.setRandom(nPnt);
	Global::setDoubleArrayData(massPlug, v);
	dTPlug.setDouble(1/24.0);

	return s;
}

bool RigSimulationNode::computeExternalForce( const EigVec& pos, const EigVec& vel, const EigVec& m, double time, EigVec& extForce )
{
	MStatus s;
	MPlug posPlug = Global::getPlug(this, m_fieldDataPositionName[0]);
	MPlug velPlug = Global::getPlug(this, m_fieldDataVelocityName[0]);
	MPlug massPlug= Global::getPlug(this, m_fieldDataMassName[0]);
	MPlug dTPlug  = Global::getPlug(this, m_fieldDataDeltaTimeName[0]);

	s = Global::setVectorArrayData(posPlug, pos);
	CHECK_MSTATUS_AND_RETURN(s,false);
	s = Global::setVectorArrayData(velPlug, vel);
	CHECK_MSTATUS_AND_RETURN(s,false);
	s = Global::setDoubleArrayData(massPlug, m);
	CHECK_MSTATUS_AND_RETURN(s,false);
	s = dTPlug.setDouble(time);
	CHECK_MSTATUS_AND_RETURN(s,false);

	s = getInputForce(extForce);
	CHECK_MSTATUS_AND_RETURN(s,false);
	return true;
}

bool RigSimulationNode::getControlParams( EigVec& targetParam, EigVec& propGain, EigVec& deriGain )
{
	int nParam = getNumParam();
	if (nParam == 0)
		return false;

	targetParam.resize(nParam);
	propGain.resize(nParam);
	deriGain.resize(nParam);

	MStatus s;
	MPlug targetParamArrayPlug = Global::getPlug(this, m_targetParamName[0]);
	MPlug propGainArrayPlug    = Global::getPlug(this, m_proportionalGainName[0]);
	MPlug deriGainArrayPlug    = Global::getPlug(this, m_derivativeGainRatioName[0]);
	MPlug stepTimePlug         = Global::getPlug(this, m_timeStepName[0]);

	for (int ithParam = 0; ithParam < nParam; ++ithParam)
	{
		MPlug targetParamPlug = targetParamArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);
		MPlug propGainPlug    = propGainArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);
		MPlug deriGainPlug    = deriGainArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);

		targetParamPlug.getValue(targetParam[ithParam]);
		propGainPlug.getValue(propGain[ithParam]);
		deriGainPlug.getValue(deriGain[ithParam]);
	}

	// 由于deriGain用的是比率形式，所以最后要相乘
	// deriGain = propGain * deriGainRatio * dt
	double dt = stepTimePlug.asDouble();
	deriGain = propGain.cwiseProduct(deriGain) * dt;
	return true;
}

bool RigSimulationNode::getControlTarget( EigVec& targetParam )
{
	int nParam = getNumParam();
	if (nParam == 0)
		return false;

	targetParam.resize(nParam);

	MStatus s;
	MPlug targetParamArrayPlug = Global::getPlug(this, m_targetParamName[0]);

	for (int ithParam = 0; ithParam < nParam; ++ithParam)
	{
		MPlug targetParamPlug = targetParamArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);

		targetParamPlug.getValue(targetParam[ithParam]);
	}
	return true;
}

bool RigSimulationNode::getControlGain( EigVec& propGain, EigVec& deriGain )
{
	int nParam = getNumParam();
	if (nParam == 0)
		return false;

	propGain.resize(nParam);
	deriGain.resize(nParam);

	MStatus s;
	MPlug propGainArrayPlug    = Global::getPlug(this, m_proportionalGainName[0]);
	MPlug deriGainArrayPlug    = Global::getPlug(this, m_derivativeGainRatioName[0]);
	MPlug stepTimePlug         = Global::getPlug(this, m_timeStepName[0]);

	for (int ithParam = 0; ithParam < nParam; ++ithParam)
	{
		MPlug propGainPlug    = propGainArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);
		MPlug deriGainPlug    = deriGainArrayPlug.elementByLogicalIndex(ithParam, &s);
		CHECK_MSTATUS_AND_RETURN(s, false);

		propGainPlug.getValue(propGain[ithParam]);
		deriGainPlug.getValue(deriGain[ithParam]);
	}

	// 由于deriGain用的是比率形式，所以最后要相乘
	// deriGain = propGain * deriGainRatio * dt
	double dt = stepTimePlug.asDouble();
	deriGain = propGain.cwiseProduct(deriGain) * dt;
	return true;
}
















