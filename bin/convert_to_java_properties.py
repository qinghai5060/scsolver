#!/usr/bin/env python

import sys

nameMap = {
    'SCSOLVER_STR_MAINDLG_TITLE': 'SolverDialog.Title',
    'SCSOLVER_STR_DEFINE_MODEL': 'SolverDialog.flModel.Label',
    'SCSOLVER_STR_SET_TARGET_CELL': 'SolverDialog.ftTargetCell.Label',
    'SCSOLVER_STR_GOAL': 'SolverDialog.ftObj.Label',
    'SCSOLVER_STR_MAXIMIZE': 'SolverDialog.rbMax.Label',
    'SCSOLVER_STR_MINIMIZE': 'SolverDialog.rbMin.Label',
    'SCSOLVER_STR_DECISIONVAR_CELLS': 'SolverDialog.ftDecVars.Label',
    'SCSOLVER_STR_CONSTRAINT_SEP': 'SolverDialog.flConstraints.Label',
    'SCSOLVER_STR_CONSTRAINTDLG_TITLE': 'ConstEditDialog.Title',
    'SCSOLVER_STR_CELL_REFERENCE': 'ConstEditDialog.ftLeft.Label',
    'SCSOLVER_STR_CONSTRAINT': 'ConstEditDialog.ftRight.Label',
    'SCSOLVER_STR_BTN_OK': 'Common.OK.Label',
    'SCSOLVER_STR_BTN_CANCEL': 'Common.Cancel.Label',
    'SCSOLVER_STR_MSG_REF_CON_RANGE_MISMATCH': 'Common.ConstRangeMismatch.Label',
    'SCSOLVER_STR_BTN_ADD': 'SolverDialog.btnConstAdd.Label',
    'SCSOLVER_STR_BTN_CHANGE': 'SolverDialog.btnConstChange.Label',
    'SCSOLVER_STR_BTN_DELETE': 'SolverDialog.btnConstDelete.Label',
    'SCSOLVER_STR_BTN_SOLVE': 'SolverDialog.btnSolve.Label',
    'SCSOLVER_STR_BTN_RESET': 'SolverDialog.btnReset.Label',
    'SCSOLVER_STR_BTN_OPTIONS': 'SolverDialog.btnOptions.Label',
    'SCSOLVER_STR_BTN_SAVE_MODEL': 'SolverDialog.btnSave.Label',
    'SCSOLVER_STR_BTN_LOAD_MODEL': 'SolverDialog.btnLoad.Label',
    'SCSOLVER_STR_BTN_CLOSE': 'SolverDialog.btnClose.Label',
    'SCSOLVER_STR_MSG_SOLUTION_NOT_FOUND': 'Common.SolutionNotFound.Label',
    'SCSOLVER_STR_MSG_SOLUTION_FOUND': 'Common.SolutionFound.Label',
    'SCSOLVER_STR_MSG_CELL_GEOMETRIES_DIFFER': 'Common.CellGeometriesDiffer.Label',
    'SCSOLVER_STR_MSG_MAX_ITERATION_REACHED': 'Common.MaxIterationReached.Label',
    'SCSOLVER_STR_MSG_STD_EXCEPTION_CAUGHT': 'Common.StdException.Label',
    'SCSOLVER_STR_MSG_ITERATION_TIMED_OUT': 'Common.IterationTimedOut.Label',
    'SCSOLVER_STR_MSG_GOAL_NOT_SET': 'Common.GoalNotSet.Label',
    'SCSOLVER_STR_OPTIONDLG_TITLE': 'OptionDialog.Title',
    'SCSOLVER_STR_OPTION_ASSUME_LINEAR': 'OptionDialog.cbLinear.Label',
    'SCSOLVER_STR_OPTION_VAR_POSITIVE': 'OptionDialog.cbPositiveValue.Label',
    'SCSOLVER_STR_OPTION_VAR_INTEGER': 'OptionDialog.cbIntegerValue.Label',
    'SCSOLVER_STR_TARGET_NOT_SET': 'Common.TargetNotSet.Label',
    'SCSOLVER_STR_DECISIONVAR_NOT_SET': 'Common.DecisionNotSet.Label'
}

def warn (msg):
    msg = "warning: " + msg + "\n"
    sys.stderr.write(msg)

def main (args):
    if len(args) < 2:
        return

    file = open(args[1], 'r')
    lines = file.readlines()
    file.close()

    for line in lines:
        line = line.strip()
        words = line.split("\t")
        if len(words) != 2:
            # skip abnormal lines.
            warn('skipping an abnormal line: "%s"'%line)
            continue

        if not nameMap.has_key(words[0]):
            warn("unknown name %s"%words[0])
            continue

        name = nameMap[words[0]]
        print (name + "=" + words[1])
        





if __name__ == '__main__':
    main(sys.argv)

