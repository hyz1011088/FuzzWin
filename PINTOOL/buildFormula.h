#pragma once

#include "TaintManager.h"
#include <sstream>
#if DEBUG
#include <iostream> // cout
#endif

class SolverFormula
{
private:
    std::ostringstream m_formula;

    // index pour les contraintes
    UINT32 m_iAssert; 

    // index pour les variables de 1/8/16/32/64/128 bits
    UINT32 m_iTbit, m_iTb, m_iTw, m_iTdw, m_iTqw, m_iTdqw;

    // proc�dure de d�claration d'un objet (r�cursif)
    void declareObject(const TaintPtr &tPtr);  

    // procedure de declaration d'une relation entre un objet et ses sources
    void declareRelation(const TaintPtr &tPtr, const vector<ObjectSource> &sources);

    // insertion dans la formule de l'ent�te d'une nouvelle contrainte
    // ie n� de contrainte, addresse, et type de condition (si mode DEBUG)
    void declareConstraintHeader(ADDRINT insAddress, PREDICATE p);

    // insertion dans la formule du final d'une nouvelle contrainte
    // ie la commande "assert" puis true ou false selon branche prise ou non
    void declareConstraintFooter(const std::string &number, bool taken);

    // insere dans la formule le nom de l'objet 'objSrc'
    // => soit le num�ro de variable, soit une valeur num�rique
    // si pr�cis�, ins�re un espace � la fin
    void insertSourceName(std::string &out, const ObjectSource &objSrc);

public:
    SolverFormula();

    // traduit une contrainte d�pendant des flags 
    void addConstraint_OVERFLOW (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken);
    void addConstraint_PARITY   (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken);
    void addConstraint_SIGN     (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken);
    void addConstraint_ZERO     (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken);
    void addConstraint_BELOW    (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken);
    void addConstraint_LESS          (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken, ADDRINT flagsValue);
    void addConstraint_LESS_OR_EQUAL (TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken, ADDRINT flagsValue);
    void addConstraint_BELOW_OR_EQUAL(TaintManager_Thread *pTmgrTls, ADDRINT insAddress, bool isTaken, ADDRINT flagsValue);

    // fabrication de la formule finale, et envoi dans le pipe
    void final();
};

extern SolverFormula *pFormula;