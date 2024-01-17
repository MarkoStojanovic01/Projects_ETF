import mongoose from "mongoose";

const Schema = mongoose.Schema;

let Appointment = new Schema({
    doc_firstname: {
        type: String
    },
    doc_lastname: {
        type: String
    },
    doc_username: {
        type: String
    },
    patient: {
        type: String
    },
    examination: {
        type: String
    },
    price: {
        type: String
    },
    duration: {
        type: Number
    },
    specialization: {
        type: String
    },
    date: {
        type: Date
    },
    branch: {
        type: String
    }
})

export default mongoose.model('AppointmentModel', Appointment, 'appointments')