/*===========================================================================
 *
 * File:		Srrecorddialog.H
 * Author:		Dave Humphrey (dave@uesp.net)
 * Created On:	26 November 2011
 *
 * Description
 *
 *=========================================================================*/
#ifndef __SRRECORDDIALOG_H
#define __SRRECORDDIALOG_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
	#include "modfile/srespfile.h"
	#include "modfile/srmultirecordhandler.h"
	#include "windows/srrecordlistctrl.h"
	#include "windows/tabctrlsheet.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Macros to help define a UI field map */
  #define DECLARE_SRRECUIFIELDS()	static sruirecfields_t s_UIRecFields[]; \
	 virtual sruirecfields_t* GetUIFields (void);

  #define BEGIN_SRRECUIFIELDS(Class) sruirecfields_t* Class::GetUIFields (void) { return Class::s_UIRecFields; } \
		sruirecfields_t Class::s_UIRecFields[] = { 

  #define ADD_SRRECUIFIELDS(Field, ID, MaxLength, ToolID)  { Field, ID, MaxLength, ToolID },
  #define END_SRRECUIFIELDS()  { SR_FIELD_NONE, 0, 0, 0 } }; 

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Type Definitions
 *
 *=========================================================================*/

	/* Holds the UI field information */
  struct sruirecfields_t 
  {
	srrecfieldid_t	FieldID;
	int				ControlID;
	int				MaxLength;
	int				ToolTipID;
  };

		/* Forward class definitions */
	class CSrEditDlgHandler;

		/* Holds a copy of a record's condition */
	struct srconditioninfo_t
	{
		CSrCtdaSubrecord	Condition;
		CSrStringSubrecord* pParam1;
		CSrStringSubrecord* pParam2;

		srconditioninfo_t()
		{
			Condition.Initialize(SR_NAME_CTDA, SR_CTDA_SUBRECORD_SIZE);
			Condition.InitializeNew();
			pParam1 = NULL;
			pParam2 = NULL;
		}

		~srconditioninfo_t()
		{
			delete pParam1;
			delete pParam2;
		}

		void Copy (srconditioninfo_t& Source)
		{
			delete pParam1;
			pParam1 = NULL;
			delete pParam2;
			pParam2 = NULL;

			Condition.Copy(&Source.Condition);

			if (Source.pParam1)
			{
				pParam1 = new CSrStringSubrecord;
				pParam1->Initialize(SR_NAME_CIS1, 0);
				pParam1->Copy(Source.pParam1);
			}

			if (Source.pParam2)
			{
				pParam2 = new CSrStringSubrecord;
				pParam2->Initialize(SR_NAME_CIS2, 0);
				pParam2->Copy(Source.pParam2);
			}

		}

		void CopyParam1 (CSrSubrecord* pSubrecord)
		{
			if (pSubrecord == NULL) return;

			if (pParam1 == NULL) 
			{
				pParam1 = new CSrStringSubrecord;
				pParam1->Initialize(SR_NAME_CIS1, 0);
			}

			pParam1->Copy(pSubrecord);
		}

		void CopyParam2 (CSrSubrecord* pSubrecord)
		{
			if (pSubrecord == NULL) return;

			if (pParam2 == NULL) 
			{
				pParam2 = new CSrStringSubrecord;
				pParam2->Initialize(SR_NAME_CIS2, 0);
			}

			pParam2->Copy(pSubrecord);
		}

		bool SetIfValidParam1 (const char* pBuffer)
		{
			if (pBuffer != NULL && pBuffer[0] == ':' && pBuffer[1] == ':')
			{
				if (pParam1 == NULL) 
				{
					pParam1 = new CSrStringSubrecord;
					pParam1->Initialize(SR_NAME_CIS1, 0);
				}

				pParam1->SetString(pBuffer);
				return true;
			}

			delete pParam1;
			pParam1 = NULL;
			return false;
		}

		bool SetIfValidParam2 (const char* pBuffer)
		{
			if (pBuffer != NULL && pBuffer[0] == ':' && pBuffer[1] == ':')
			{
				if (pParam2 == NULL) 
				{
					pParam2 = new CSrStringSubrecord;
					pParam2->Initialize(SR_NAME_CIS2, 0);
				}

				pParam2->SetString(pBuffer);
				return true;
			}

			delete pParam2;
			pParam2 = NULL;
			return false;
		}
	};

	typedef CSrPtrArray<srconditioninfo_t> CSrConditionArray;

/*===========================================================================
 *		End of Type Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CSrRecordDialog Definition
 *
 * Description
 *
 *=========================================================================*/
class CSrRecordDialog : public CFormView, public ISrListener {
  DECLARE_DYNCREATE(CSrRecordDialog);
  DECLARE_SRRECUIFIELDS();

  /*---------- Begin Protected Class Members --------------------*/
protected:
  sreditrecinfo_t			m_EditInfo;		/* Holds information on the record being edited */

public:
  CSrMultiRecordHandler*	m_pRecordHandler;	/* Main record handler object */

protected:
  HACCEL				m_hAccelerator;		/* Optional custom accelerator table for the dialog */
  int					m_AcceleratorID;

  CSrEditDlgHandler*	m_pDlgHandler;

  CString		m_TitlePrefix;		/* Used to output the record type to the window title */

  CWnd*			m_pEditorIDField;
  CWnd*			m_pBipedPartsField;
  CWnd*			m_pScriptField;
  dword			m_ScriptType;
  CWnd*			m_pEnchantField;
  dword			m_EnchantType;
  CWnd*			m_pSoundField;
  CWnd*			m_pConditionField;

  CWnd*			m_pModelField;
  CWnd*			m_pIconField;
  CWnd*			m_pSoundFileField;  
  CWnd*			m_pDropSoundField;  
  CWnd*			m_pPickupSoundField;  
  CWnd*			m_pMaleWorldModelField;
  CWnd*			m_pMaleBipedModelField;
  CWnd*			m_pFemaleWorldModelField;
  CWnd*			m_pFemaleBipedModelField;
  CWnd*			m_pMaleIconField;
  CWnd*			m_pFemaleIconField;
  CWnd*			m_pBoundsField;
  CWnd*			m_pKeywordsField;
  srboundsdata_t m_BoundsCopy;

  CTabCtrlSheet*	m_pTabControl;

  bool			m_InitialSetData;
  bool			m_NoActivateRecord;

  CToolTipCtrl		m_ToolTip;

  CSrConditionArray	m_ConditionsCopy;
  bool				m_ConditionsChanged;
  bool				m_IgnoreConditions;


  /*---------- Begin Protected Class Methods --------------------*/
protected:

	virtual void CopyConditions (void);
	virtual void SaveConditions (void);

	virtual srconditioninfo_t* CreateConditionInfo (CSrCtdaSubrecord* pCondition, CSrRecord* pRecord, const dword Index);

		/* Output/input defined fields */
	void GetUIFieldData (void);
	void SetUIFieldData (void);
	bool GetKeywords(sruirecfields_t& FieldInfo, CListBox* pListBox);
	bool SetKeywords(sruirecfields_t& FieldInfo, CListBox* pListBox);

		/* Update the editor ID in the edit info structure */
	virtual int UpdateEditorID (void);

		/* Initialize the dialog tooltips */
	virtual void InitToolTips (void);


  /*---------- Begin Public Class Methods -----------------------*/
public:

	/* Class Constructors/Destructors */
  CSrRecordDialog (const int ID = 0);
  //virtual ~CSrRecordDialog() { Destroy(); }
  //virtual void Destroy (void);

	/* Close the window */
  virtual void Close (void);

  void AddTabControl (CTabCtrlSheet& TabCtrl) { m_pTabControl = &TabCtrl; }

	/* Access the UI field array */
  //virtual sruirecfields_t* GetUIFields (void) { return (NULL); }

  int DropRecordHelper (srrldroprecords_t* pDropItems, CWnd* pWnd, const srrectype_t AllowedType, const int AllowedCount);
  int DropRecordHelper (srrldroprecords_t* pDropItems, CListBox& ListBox, const srrectype_t AllowedType, const bool PermitDuplicates);

	/* Access the dialog record object */
  virtual CSrRecord*       GetInputRecord  (void) { return (m_EditInfo.pOldRecord); }
  virtual CSrRecord*       GetOutputRecord (void) { return (m_EditInfo.pNewRecord); }
  virtual sreditrecinfo_t& GetEditInfo     (void) { return (m_EditInfo); }

	/* Return a prefix describing the record type */
  virtual const SSCHAR* GetTitlePrefix (void) { return (m_TitlePrefix); }

	/* Check if the dialog is editing the given record */
  virtual bool IsEditRecord (CSrRecord* pRecord) { return (m_EditInfo.pOldRecord == pRecord); }

  	/* Set and get control data */
  virtual void  ClearControlData (void);
  virtual void  GetControlData   (void);
  virtual void  SetControlData   (void);
  virtual void  SetTitle         (const SSCHAR* pEditorID);

	/* Main Events */
  virtual int  OnPreSaveRecord    (void);
  virtual int  OnPostSaveRecord   (void);
  virtual int  OnCancelEditRecord (void);
  virtual void OnApply            (void);
  virtual void OnSave			  (void);
  virtual void OnCancel			  (void);

		/* Listener events */
  virtual int OnListenAddRecord       (CSrListenEvent* pEvent) { return (0); }
  virtual int OnListenCleanRecord     (CSrListenEvent* pEvent) { return (0); }
  virtual int OnListenUpdateRecord    (CSrListenEvent* pEvent) { return (0); }
  virtual int OnListenPreUpdateRecord (CSrListenEvent* pEvent) { return (0); }
  virtual int GetListenEvents         (void)                   { return (0); }

  		/* Set class members */
  virtual void SetEditInfo      (sreditrecinfo_t&       InputInfo);
  virtual void SetRecordHandler (CSrMultiRecordHandler* pHandler) { m_pRecordHandler = pHandler; }
  virtual void SetDlgHandler    (CSrEditDlgHandler*     pHandler) { m_pDlgHandler    = pHandler; }
  virtual void SetTitlePrefix   (const TCHAR*           pString)  { m_TitlePrefix    = pString; }


		/* ClassWizard generated virtual function overrides */
public:
  virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
  virtual void OnInitialUpdate();
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
  virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);


protected:

		/* Generated message map functions */
  afx_msg void OnDestroy();
  afx_msg void OnKillfocusEditorid();
  afx_msg void OnBipedpartsButton();
  afx_msg void OnEditScript();
  afx_msg void OnEditEnchantment();
  afx_msg void OnSelectScript();
  afx_msg void OnSelectEnchantment();
  afx_msg LRESULT OnEditRecordMsg (WPARAM wParam, LPARAM lParam);
  afx_msg void OnDropScript (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropEnchant (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnEditCut (void);
  afx_msg void OnEditUndo (void);
  afx_msg void OnEditCopy (void);
  afx_msg void OnEditPaste (void);
  afx_msg void OnEditSelectAll (void);
  afx_msg void OnUpdateEditSelectAll (CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditCopy (CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditPaste (CCmdUI* pCmdUI);
  afx_msg BOOL OnHelpInfo (HELPINFO* pHeplInfo);
  afx_msg void OnHelp (void);
  afx_msg void OnDropModel (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropMaleWorldModel (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropMaleBipedModel (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropFemaleWorldModel (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropFemaleBipedModel (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropIcon (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropMaleIcon (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropFemaleIcon (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnDropSoundFile (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnSelectModel (void);
  afx_msg void OnSelectIcon (void);
  afx_msg void OnSelectSoundFile (void);
  afx_msg void OnSelectMaleWorldModel (void);
  afx_msg void OnSelectMaleBipedModel (void);
  afx_msg void OnSelectMaleIcon (void);
  afx_msg void OnSelectFemaleWorldModel (void);
  afx_msg void OnSelectFemaleBipedModel (void);
  afx_msg void OnSelectFemaleIcon (void);
  afx_msg void OnSelectsoundButton();
  afx_msg void OnEditSound();
  afx_msg void OnDropSound (NMHDR* pNotifyStruct, LRESULT* pResult);
  afx_msg void OnBnClickedAddkeywordButton();
  afx_msg void OnBnClickedEditkeywordButton();
  afx_msg void OnBnClickedDelkeywordButton();
  afx_msg void OnLbnSelchangeKeywords();
  afx_msg void OnBnClickedConditionButton();
	afx_msg void OnBnClickedSelectdropsoundButton();
	afx_msg void OnBnClickedSelectpickupButton();
	afx_msg void OnBnClickedEditDropsound();
	afx_msg void OnBnClickedEditPickupsound();
	afx_msg void OnDropPickupSound (NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnDropDropSound (NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnDropKeywords (NMHDR* pNotifyStruct, LRESULT* pResult);

	DECLARE_MESSAGE_MAP();

 public:
	 afx_msg void OnEditFind();
	 afx_msg void OnBnClickedBounds();
};
/*===========================================================================
 *		End of Class CSrRecordDialog Definition
 *=========================================================================*/


#endif
/*===========================================================================
 *		End of File SrRecordDialog.H
 *=========================================================================*/
