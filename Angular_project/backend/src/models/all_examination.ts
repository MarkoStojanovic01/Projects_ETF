import mongoose from "mongoose";

const Schema = mongoose.Schema;

let AllExamination = new Schema({
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
    added: {
        type: String
    }
})

export default mongoose.model('AllExaminationModel', AllExamination, 'all_examinations')