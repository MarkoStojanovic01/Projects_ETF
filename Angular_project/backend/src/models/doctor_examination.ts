import mongoose from "mongoose";

const Schema = mongoose.Schema;

let DoctorExamination = new Schema({
    doctor: {
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
    }
})

export default mongoose.model('DoctorExaminationModel', DoctorExamination, 'doctor_examinations')