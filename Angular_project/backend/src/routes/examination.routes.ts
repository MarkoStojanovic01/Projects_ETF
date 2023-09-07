import express from 'express';
import { ExaminationController } from '../controllers/examination.controller';
const examinationRouter = express.Router();

examinationRouter.route('/getDocExaminations').post(
    (req, res) => new ExaminationController().getDocExaminations(req, res)
)

examinationRouter.route('/getDocAppointments').post(
    (req, res) => new ExaminationController().getDocAppointments(req, res)
)

examinationRouter.route('/getPatientDocAppointments').post(
    (req, res) => new ExaminationController().getPatientDocAppointments(req, res)
)


examinationRouter.route('/addDocAppointment').post(
    (req, res) => new ExaminationController().addDocAppointment(req, res)
)

examinationRouter.route('/removeDocAppointment').post(
    (req, res) => new ExaminationController().removeDocAppointment(req, res)
)

examinationRouter.route('/getPatientAppointments').post(
    (req, res) => new ExaminationController().getPatientAppointments(req, res)
)

examinationRouter.route('/getPatientReports').post(
    (req, res) => new ExaminationController().getPatientReports(req, res)
)

examinationRouter.route('/getPatientDocReports').post(
    (req, res) => new ExaminationController().getPatientDocReports(req, res)
)

examinationRouter.route('/getSpecExaminations').post(
    (req, res) => new ExaminationController().getSpecExaminations(req, res)
)

examinationRouter.route('/requestExamination').post(
    (req, res) => new ExaminationController().requestExamination(req, res)
)

examinationRouter.route('/addDocExamination').post(
    (req, res) => new ExaminationController().addDocExamination(req, res)
)

examinationRouter.route('/addReport').post(
    (req, res) => new ExaminationController().addReport(req, res)
)

examinationRouter.route('/getAllSpecializations').get(
    (req, res) => new ExaminationController().getAllSpecializations(req, res)
)



export default examinationRouter;