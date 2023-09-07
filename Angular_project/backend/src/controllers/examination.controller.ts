import * as express from 'express';
import DoctorExamination from '../models/doctor_examination';
import Appointment from '../models/appointment';
import Report from '../models/report';
import AllExamination from '../models/all_examination';
import Specialization from '../models/specialization';

export class ExaminationController {

    getDocExaminations = (req: express.Request, res: express.Response)=>{
        let username = req.body.username;

        DoctorExamination.find({'doctor': username}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getDocAppointments = (req: express.Request, res: express.Response)=>{
        let doc = req.body.doc;

        Appointment.find({'doc_username': doc}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    addDocAppointment = (req: express.Request, res: express.Response)=>{
        let appointment = new Appointment({
            examination: req.body.examination,
            specialization: req.body.specialization,
            duration: req.body.duration,
            price: req.body.price,
            doc_username: req.body.doc_username,
            doc_firstname: req.body.doc_firstname,
            doc_lastname: req.body.doc_lastname,
            patient: req.body.patient,
            date: req.body.date,
            branch: req.body.branch
        })

        appointment.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })

    }

    getPatientAppointments = (req: express.Request, res: express.Response)=>{
        let patient = req.body.patient;

        Appointment.find({'patient': patient}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getPatientDocAppointments = (req: express.Request, res: express.Response)=>{
        let patient = req.body.patient;
        let doc = req.body.doc;

        Appointment.find({'patient': patient,'doc_username': doc}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getPatientReports = (req: express.Request, res: express.Response)=>{
        let patient = req.body.patient;

        Report.find({'patient': patient}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getPatientDocReports = (req: express.Request, res: express.Response)=>{
        let patient = req.body.patient;
        let doc = req.body.doc;

        Report.find({'patient': patient,'doc_username': doc}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    removeDocAppointment = (req: express.Request, res: express.Response)=>{
        let doc = req.body.doc;
        let date = req.body.date;

        Appointment.deleteOne({'doc_username': doc, 'date': date}, (err, news)=>{
            if(err) console.log(err);
            else res.json({'message': 'ok'})
        })

    }

    getSpecExaminations = (req: express.Request, res: express.Response)=>{
        let specialization = req.body.specialization;

        AllExamination.find({'specialization': specialization, 'added': 'true'}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    requestExamination = (req: express.Request, res: express.Response)=>{
        let examination = new AllExamination({
            examination: req.body.examination,
            specialization: req.body.specialization,
            duration: req.body.duration,
            price: req.body.price,
            added: 'false'
        })

        examination.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })


    }

    addDocExamination = (req: express.Request, res: express.Response)=>{
        let examination = new DoctorExamination({
            examination: req.body.examination,
            specialization: req.body.specialization,
            duration: req.body.duration,
            price: req.body.price,
            doctor: req.body.doc
        })

        examination.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })

    }

    addReport = (req: express.Request, res: express.Response)=>{
        let report = new Report({
            specialization: req.body.specialization,
            doc_username: req.body.doc_username,
            doc_firstname: req.body.doc_firstname,
            doc_lastname: req.body.doc_lastname,
            patient: req.body.patient,
            date: req.body.date,
            diagnosis: req.body.diagnosis,
            reason: req.body.reason,
            therapy: req.body.therapy,
            nextDate: req.body.nextDate
        })

        report.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    getAllSpecializations = (req: express.Request, res: express.Response)=>{

        Specialization.find( (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

}