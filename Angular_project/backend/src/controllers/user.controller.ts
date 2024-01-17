import * as express from 'express';
import User from '../models/user'
import Rejected from '../models/rejected'

export class UserController {
    login = (req: express.Request, res: express.Response) => {
        let username = req.body.username;
        let password = req.body.password;

        User.findOne({ 'username': username, 'password': password, 'added':true}, (err, user) => {
            if (err) console.log(err);
            else res.json(user)
        })
    }

    getUser = (req: express.Request, res: express.Response) => {
        let username = req.body.username;

        User.findOne({ 'username': username, 'added':true }, (err, user) => {
            if (err) console.log(err);
            else res.json(user)
        })
    }

    getAllDoctors = (req: express.Request, res: express.Response)=>{
        User.find({'type': 'doctor'}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getAllUsers = (req: express.Request, res: express.Response)=>{
        User.find({'type': ['patient', 'doctor'], 'added': true}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    getNonRegistered = (req: express.Request, res: express.Response)=>{
        User.find({'type': 'patient', 'added': false}, (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }

    deleteUser = (req: express.Request, res: express.Response)=>{
        let username = req.body.username;

        User.deleteOne({'username': username, 'added': true}, (err, news)=>{
            if(err) console.log(err);
            else res.json({'message': 'User successfully deleted!'})
        })

    }

    addUser = (req: express.Request, res: express.Response)=>{
        let user = new User({
            username: req.body.username,
            password: req.body.password,
            firstname: req.body.firstname,
            lastname: req.body.lastname,
            specialization: req.body.specialization,
            type: req.body.type,
            adress: req.body.adress,
            branch: req.body.branch,
            licenceNumber: req.body.licenceNumber,
            mail: req.body.mail,
            phone: req.body.phone,
            image: req.body.image,
            added: req.body.added
        })

        user.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    updatePatient = (req: express.Request, res: express.Response) => {
        let username = req.body.username;
        let firstname = req.body.firstname;
        let lastname = req.body.lastname;
        let adress = req.body.adress;
        let mail = req.body.mail;
        let phone = req.body.phone;

        User.findOneAndUpdate({ 'username': username, 'type':'patient', added :true},{'firstname':firstname,
        'lastname':lastname,'adress':adress, 'phone':phone, 'mail':mail }, (err, resp) => {
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    updateDoctor = (req: express.Request, res: express.Response) => {
        let username = req.body.username;
        let firstname = req.body.firstname;
        let lastname = req.body.lastname;
        let adress = req.body.adress;
        let licence = req.body.licence;
        let phone = req.body.phone;
        let specialization = req.body.specialization;

        User.findOneAndUpdate({ 'username': username, 'type':'doctor', added :true},{'firstname':firstname,
        'lastname':lastname,'adress':adress, 'phone':phone, 'licence':licence, 'specialization':specialization }, (err, resp) => {
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    addImage = (req: express.Request, res: express.Response) => {
        let username = req.body.username;
        let image = req.body.image;

        User.findOneAndUpdate({ 'username': username, 'added':true},{'image':image}, (err, resp) => {
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    updatePassword = (req: express.Request, res: express.Response) => {
        let username = req.body.username;
        let password = req.body.password;

        User.findOneAndUpdate({ 'username': username, 'added':true},{'password':password}, (err, resp) => {
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"ok"})
        })
    }

    acceptPatient = (req: express.Request, res: express.Response) => {
        let username = req.body.username;

        User.findOneAndUpdate({ 'username': username, 'added':false},{'added':true}, (err, resp) => {
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
            else res.json({"message":"Patient accepted"})
        })
    }

    removePatient = (req: express.Request, res: express.Response)=>{
        let username = req.body.username;
        let mail = req.body.mail;

        let rejected = new Rejected({
            username: username,
            mail: mail
        })

        rejected.save((err,resp)=>{
            if(err){
                console.log(err);
                res.status(400).json({"message":"error"})
            }
        })

        User.deleteOne({'username': username, 'added': false}, (err, news)=>{
            if(err) console.log(err);
            else res.json({'message': 'Patient successfully removed!'})
        })

    }

    getRejected = (req: express.Request, res: express.Response)=>{
        Rejected.find( (err, news)=>{
            if(err) console.log(err)
            else res.json(news)
        })

    }
    

}