import mongoose from "mongoose";

const Schema = mongoose.Schema;

let Report = new Schema({
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
    diagnosis: {
        type: String
    },
    reason: {
        type: String
    },
    specialization: {
        type: String
    },
    date: {
        type: Date
    },
    therapy: {
        type: String
    },
    nextDate: {
        type: Date
    }
})

export default mongoose.model('ReportModel', Report, 'reports')